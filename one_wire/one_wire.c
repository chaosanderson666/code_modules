/*
 *********************************************************************************************************
 *                                           Single Wire Bus
 *
 *                            (c) Copyright 1999, John, All Rights Reserved
 *
 *********************************************************************************************************
 */
#include <stdint.h>
#include "1-wire.h"
/*
 *********************************************************************************************************
 */
#define   RELEASE_BUS()      //set_gpio()
#define   PULL_BUS_LOW()     //reset_gpio()
#define   READ_BUS()         //read_gpio()
#define   DELAY_US(tick)     delay_us(tick)  /* Pause for exactly 'tick' number of ticks = 0.25us */
/*
 *********************************************************************************************************
 */
static uint16_t A, B, C, D, E, F, G, H, I, J;  /* 'tick' values */
/*
*********************************************************************************************************
* Description : Set the 1-Wire timing to 'standard' (standard = 1) or 'overdrive' (standard = 0).
*********************************************************************************************************
*/
void ow_setspeed(uint8_t standard)
{
    /* ticks = time(us) * 4 */
	if(standard == 0){
        /* Standard Speed */  
		A = 24;   /* ie. 6us * 4 = 24ticks. */
		B = 256;
		C = 240;
		D = 40;
		E = 36;
		F = 220;
		G = 0;
		H = 1920;
		I = 280;
		J = 1640;
	}else{             
	    /* Overdrive Speed */
		A = 6;
		B = 30;
		C = 30;
		D = 10;
		E = 3;
		F = 28;
		G = 10;
		H = 280;
		I = 34;
		J = 160;
	}
}
/*
*********************************************************************************************************
* Description : Generate a 1-Wire reset, return 1 if no presence detect was found, return 0 otherwise.
*********************************************************************************************************
*/
uint8_t ow_reset(void)
{
	uint8_t ret;

	DELAY_US(G);
	PULL_BUS_LOW();
	DELAY_US(H);
	RELEASE_BUS();
	DELAY_US(I);
	ret = READ_BUS();  /* Sample for presence pulse from slave. */
	DELAY_US(J);       /* Complete the reset sequence recovery. */
    
	return ret;        /* Return sample presence pulse result.  */
}
/*
*********************************************************************************************************
* Description : Send a 1-Wire write bit. Provide 10us recovery time.
*********************************************************************************************************
*/
static void ow_writebit(uint8_t bit)
{
	if(bit){            
        /* Write '1' bit */
    	PULL_BUS_LOW();  
		DELAY_US(A);
    	RELEASE_BUS();
		DELAY_US(B);    /* Complete the time slot and 10us recovery */
	}else{              
	    /* Write '0' bit */ 
    	PULL_BUS_LOW();
		DELAY_US(C);
	    RELEASE_BUS();
		DELAY_US(D);
	}
}
/*
*********************************************************************************************************
* Description : Read a bit from the 1-Wire bus and return it. Provide 10us recovery time.
*********************************************************************************************************
*/
static uint8_t ow_readbit(void)
{
	uint8_t ret;

    PULL_BUS_LOW();
	DELAY_US(A);
	RELEASE_BUS();
	DELAY_US(E);
	ret = READ_BUS();  /* Sample for presence pulse from slave.    */
	DELAY_US(F);       /* Complete the time slot and 10us recovery */

	return ret;
}
/*
*********************************************************************************************************
* Description : Write 1-Wire data byte.
*********************************************************************************************************
*/
void ow_writebyte(uint8_t data)
{
	uint8_t loop;

	/* Loop to write each bit in the byte, LS-bit first. */
	for (loop = 0; loop < 8; loop++){
		ow_writebit(data & 0x01);
		data >>= 1;
	}
}
/*
*********************************************************************************************************
* Description : Read 1-Wire data byte and return it.
*********************************************************************************************************
*/
uint8_t ow_readbyte(void)
{
	uint8_t loop, result = 0;

	for (loop = 0; loop < 8; loop++){
		result >>= 1;
		/* if result is one, then set MS bit */
		if (ow_readbit() == 1)
			result |= 0x80;
	}
	return result;
}
/*
*********************************************************************************************************
* Description : Write a 1-Wire data byte and return the sampled result.
*********************************************************************************************************
*/
uint8_t ow_writeread(uint8_t data)
{
	uint8_t loop, result = 0;

	for(loop = 0; loop < 8; loop++){
		result >>= 1;
		/* If sending a '1' then read a bit else write a '0'. */
		if(data & 0x01){
			if(ow_readbit() == 1)
				result |= 0x80;
		}else{
			ow_writebit(0);
        }
		data >>= 1;
	}
	return result;
}
/*
*********************************************************************************************************
* Description : Write a block 1-Wire data bytes and return the sampled result in the same buffer.
*********************************************************************************************************
*/
void ow_writeblock(uint8_t *data, uint16_t data_len)
{
	uint16_t loop;

	for (loop = 0; loop < data_len; loop++){
		data[loop] = ow_writeread(data[loop]);
	}
}
/*
 *********************************************************************************************************
 */
