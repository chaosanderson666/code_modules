/*
*********************************************************************************************************
*                                             Clock/Calendar
*
*                            (c) Copyright 1999, John All Rights Reserved
*
*********************************************************************************************************
*/
#include "i2c_simulate.h"
#include "stm32f10x.h"
//#include "systick.h"
/*
*********************************************************************************************************
*/
#define I2C_SCL GPIO_Pin_10
#define I2C_SDA GPIO_Pin_11
#define GPIO_I2C GPIOB

#define  I2C_SCL_H      GPIO_SetBits(GPIO_I2C,I2C_SCL)
#define  I2C_SCL_L      GPIO_ResetBits(GPIO_I2C,I2C_SCL)
#define  I2C_SDA_H      GPIO_SetBits(GPIO_I2C,I2C_SDA)
#define  I2C_SDA_L      GPIO_ResetBits(GPIO_I2C,I2C_SDA)
/*
*********************************************************************************************************
* Description : This function parse the command and execute it.
* Arguments   : None.
* Returns     : the execute status.
* Notes       : none.
*********************************************************************************************************
*/
void i2c_delay_us(uint8_t t)
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
uint8_t read_sda_status(void)
{
	return GPIO_ReadInputDataBit(GPIO_I2C,I2C_SDA);
}
/*
*********************************************************************************************************
* Description : This function just used to test command line.
* Arguments   : argc.
* Returns     : argv.
* Notes       : None.
*********************************************************************************************************
*/
void i2c_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin=I2C_SCL|I2C_SDA;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	I2C_SCL_H;
	I2C_SDA_H;
}
/*
*********************************************************************************************************
* Description : This function parse the command and execute it.
* Arguments   : None.
* Returns     : the execute status.
* Notes       : none.
*********************************************************************************************************
*/
void i2c_sda_out()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = I2C_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
* Description : This function parse the command and execute it.
* Arguments   : None.
* Returns     : the execute status.
* Notes       : none.
*********************************************************************************************************
*/
void i2c_sda_in(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = I2C_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/*
*********************************************************************************************************
* Description : This function just used to test command line.
* Arguments   : argc.
* Returns     : argv.
* Notes       : None.
*********************************************************************************************************
*/
void i2c_start(void)
{
	i2c_sda_out();

	I2C_SDA_H;
	I2C_SCL_H;
	i2c_delay_us(5);
	I2C_SDA_L;
	i2c_delay_us(6);
	I2C_SCL_L;
}
/*
*********************************************************************************************************
* Description : This function just used to test command line.
* Arguments   : void.
* Returns     : void.
* Notes       : None.
*********************************************************************************************************
*/
void i2c_stop(void)
{
	i2c_sda_out();

	I2C_SCL_L;
	I2C_SDA_L;
	I2C_SCL_H;
	i2c_delay_us(6);
	I2C_SDA_H;
	i2c_delay_us(6);
}
/*
*********************************************************************************************************
* Description : This function just used to test command line.
* Arguments   : argc.
* Returns     : argv.
* Notes       : None.
*********************************************************************************************************
*/
void i2c_ack(void)
{
	I2C_SCL_L;
	i2c_sda_out();
	I2C_SDA_L;
	i2c_delay_us(2);
	I2C_SCL_H;
	i2c_delay_us(5);
	I2C_SCL_L;
}

/*
*********************************************************************************************************
* Description : This function just used to test command line.
* Arguments   : argc.
* Returns     : argv.
* Notes       : None.
*********************************************************************************************************
*/
void i2c_nack(void)
{
	I2C_SCL_L;
	i2c_sda_out();
	I2C_SDA_H;
	i2c_delay_us(2);
	I2C_SCL_H;
	i2c_delay_us(5);
	I2C_SCL_L;
}
/*
*********************************************************************************************************
* Description : This function just used to test command line.
* Arguments   : argc.
* Returns     : argv.
* Notes       : None.
*********************************************************************************************************
*/
uint8_t i2c_wait_ack(void)
{
	uint8_t timeout = 0;

	i2c_sda_in();

	I2C_SDA_H;
	i2c_delay_us(1);
	I2C_SCL_H;
	i2c_delay_us(1);

	while(read_sda_status()){
		timeout++;
		if(timeout > 250){
			i2c_stop();
			
			return 1;
		}
	}

	I2C_SCL_L;

	return 0;
}
/*
*********************************************************************************************************
* Description : This function just used to test command line.
* Arguments   : argc.
* Returns     : argv.
* Notes       : None.
*********************************************************************************************************
*/
void i2c_send_byte(uint8_t txd)
{
	i2c_sda_out();
	I2C_SCL_L;

	for(uint8_t i = 0; i < 8; i++){
		if((txd & 0x80) > 0){
			I2C_SDA_H;
		}else{
			I2C_SDA_L;
		}

		txd <<= 1;
		I2C_SCL_H;
		i2c_delay_us(2);
		I2C_SCL_L;
		i2c_delay_us(2);
	}
}

/*
*********************************************************************************************************
* Description : This function just used to test command line.
* Arguments   : argc.
* Returns     : argv.
* Notes       : None.
*********************************************************************************************************
*/
uint8_t i2c_read_byte(uint8_t ack)
{
	uint8_t data = 0;

	i2c_sda_in();
	for(uint8_t i = 0; i < 8; i++)
	{
		I2C_SCL_L;
		i2c_delay_us(2);
		I2C_SCL_H;
		data <<= 1;
		
		if(read_sda_status()){
			data++;
		}
			
		i2c_delay_us(1);
	}

	if(ack == 0){
		i2c_nack();
	}else{
		i2c_ack();
	}

	return data;
}
/*
*********************************************************************************************************
*/
