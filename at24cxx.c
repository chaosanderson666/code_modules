/*
*********************************************************************************************************
*                                             Command line
*
*                            (c) Copyright 1999, John , All Rights Reserved
*********************************************************************************************************
*/
#include "i2c_simulate.h"
#include "at24cxx.h"
/*
*********************************************************************************************************
* Description : This function parse the command and execute it.
* Arguments   : None.
* Returns     : the execute status.
* Notes       : none.
*********************************************************************************************************
*/
void at24cxx_delay_us(uint8_t t)
{
	t = t;
}
/*
*********************************************************************************************************
* Description : This function parse the command and execute it.
* Arguments   : None.
* Returns     : the execute status.
* Notes       : none.
*********************************************************************************************************
*/
uint8_t at24cxx_read_one_byte(uint16_t addr)
{
	uint8_t data;

	i2c_start();

	if(EEPROM_TYPE > AT24C16){
		i2c_send_byte(0xA0);
		i2c_wait_ack();
		i2c_send_byte(addr >> 8);
	}else{
		i2c_send_byte(0xA0 + ((addr / 256) << 1));
	}
	i2c_wait_ack();

	i2c_send_byte(addr % 256);
	i2c_wait_ack();

	i2c_start();
	
	i2c_send_byte(0xA1);
	i2c_wait_ack();

	data = i2c_read_byte(0); /* 0 means nack */
	i2c_nack();
	i2c_stop();
	
	return data;
}
/*
*********************************************************************************************************
* Description : This function parse the command and execute it.
* Arguments   : None.
* Returns     : the execute status.
* Notes       : none.
*********************************************************************************************************
*/
uint16_t at24cxx_read_two_byte(uint16_t addr)
{
	uint16_t data;

	i2c_start();

	if(EEPROM_TYPE>AT24C16){
		i2c_send_byte(0xA0);
		i2c_wait_ack();
		i2c_send_byte(addr >> 8);
	}else{
		i2c_send_byte(0xA0 + ((addr / 256) << 1));
	}
	i2c_wait_ack();

	i2c_send_byte(addr % 256);
	i2c_wait_ack();

	i2c_start();

	i2c_send_byte(0xA1);
	i2c_wait_ack();

	data = i2c_read_byte(1);
	data <<= 8;
	data |= i2c_read_byte(0);

	i2c_nack();
	i2c_stop();

	return data;
}
/*
*********************************************************************************************************
* Description : This function parse the command and execute it.
* Arguments   : None.
* Returns     : the execute status.
* Notes       : none.
*********************************************************************************************************
*/
void at24cxx_write_one_byte(uint16_t addr, uint8_t data)
{
	i2c_start();

	if(EEPROM_TYPE > AT24C16){
		i2c_send_byte(0xA0);
		i2c_wait_ack();
		i2c_send_byte(addr >> 8);
	}else{
		i2c_send_byte(0xA0 + ((addr / 256) << 1));
	}
	i2c_wait_ack();

	i2c_send_byte(addr % 256);
	i2c_wait_ack();

	i2c_send_byte(data);
	i2c_wait_ack();

	i2c_stop();

	at24cxx_delay_us(10);
}
/*
*********************************************************************************************************
* Description : This function parse the command and execute it.
* Arguments   : None.
* Returns     : the execute status.
* Notes       : none.
*********************************************************************************************************
*/
void at24cxx_write_two_byte(uint16_t addr, uint16_t dt)
{
	i2c_start();

	if(EEPROM_TYPE > AT24C16){
		i2c_send_byte(0xA0);
		i2c_wait_ack();
		i2c_send_byte(addr >> 8);
	}else{
		i2c_send_byte(0xA0 + ((addr / 256) << 1));
	}
	i2c_wait_ack();
	
	i2c_send_byte(addr % 256);
	i2c_wait_ack();

	i2c_send_byte(dt >> 8);
	i2c_wait_ack();

	i2c_send_byte(dt & 0xFF);
	i2c_wait_ack();

	i2c_stop();

	at24cxx_delay_us(10);
}
/*
*********************************************************************************************************
*/
