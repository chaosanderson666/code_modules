/*
*********************************************************************************************************
*                                             Command Function
*
*                            (c) Copyright 1999, John , All Rights Reserved
*
* File name   : cmds.c
* Programmer  : John
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/
#include "stm32_eval.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cmds.h"
#include "clock.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
/*
*********************************************************************************************************
* Description : This function control the led.
* Notes       : None.
*********************************************************************************************************
*/
uint8_t led(uint8_t argc, char *argv[])
{
	if(!strcmp(argv[1], "on"))
	{
		STM_EVAL_LEDOn(LED1);
		return 0;
	}
	else if(!strcmp(argv[1], "off"))
	{
		STM_EVAL_LEDOff(LED1);
		return 0;
	}
	
	return 1;
}
/*
*********************************************************************************************************
* Description : This function show the current date and time.
* Notes       : None.
*********************************************************************************************************
*/
uint8_t show_time(uint8_t argc, char *argv[])
{
    char format_time[50];

	if(!strcmp(argv[1], "w"))
	{
		clk_set_datetime((uint16_t)atoi(argv[2]),
						(uint8_t) atoi(argv[3]),
						(uint8_t) atoi(argv[4]),
						(uint8_t) atoi(argv[5]),
						(uint8_t) atoi(argv[6]),
						(uint8_t) atoi(argv[7]));
	}
	else
	{
		clk_format_date(3, format_time);
		printf("\r\n%s", format_time);
		clk_format_time(1, format_time);
		printf(" %s", format_time);
	}
	
	return 0;
}
/*
*********************************************************************************************************
* Description : This function clear the window.
* Notes       : None.
*********************************************************************************************************
*/
uint8_t clear_window(uint8_t argc, char *argv[])
{
    printf("\x0c");
	return 0;
}

/*
*********************************************************************************************************
* Description : This function show the task state.
* Notes       : None.
*********************************************************************************************************
*/
uint8_t show_task(uint8_t argc, char *argv[])
{
	int8_t *pcWriteBuffer;
    
    pcWriteBuffer = (int8_t *)pvPortMalloc(500);
    if(NULL == pcWriteBuffer)
	{
        printf("\r\nNO MEMERY !!!");
        return 0;
    }
	printf("\r\n=================================================");
	printf("\r\nTaskName          State  Priority  FreeStack  Number");
	vTaskList((char *)pcWriteBuffer);
	printf("\r\n%s", pcWriteBuffer);
	//printf("\r\nTask       Count         Usage");
	//vTaskGetRunTimeStats((char *)&pcWriteBuffer);
	//printf("\r\n%s", pcWriteBuffer);
	printf("=================================================");
	vPortFree(pcWriteBuffer);
	printf("\r\nFree Heap Size : %d", xPortGetFreeHeapSize());
	printf("\r\nMini Free Size : %d", xPortGetMinimumEverFreeHeapSize());
	/* Obtain the current tick count. */ 
	printf("\r\nTick Count: %d", (TickType_t)xTaskGetTickCount());
	
	return 0;
}
/*
*********************************************************************************************************
* Description : Initiate a system reset request to reset the MCU.
* Notes       : None.
*********************************************************************************************************
*/
uint8_t system_reset(uint8_t argc, char *argv[])
{
	printf("\r\nSystem will reboot, NOW...\r\n");
	__set_FAULTMASK(1);
	NVIC_SystemReset();
	
	return 0;
}
/*
*********************************************************************************************************
* Description : This function show the device information.
* Notes       : None.
*********************************************************************************************************
*/
uint8_t dev_infor(uint8_t argc, char *argv[])
{
	printf("\r\nFlash size: %dKB", (uint16_t)(*((uint32_t *)0x1FFFF7E0)));
	printf("\r\nChip ID   : 0x%x%x%x%x", 
	(uint32_t)(*((uint32_t *)(0x1FFFF7E8 + 0x08))),
	(uint32_t)(*((uint32_t *)(0x1FFFF7E8 + 0x04))),
	(uint16_t)(*((uint32_t *)(0x1FFFF7E8 + 0x02))),
	(uint16_t)(*((uint32_t *)0x1FFFF7E8)));
	printf("\r\nCPU ID    : 0x%x", SCB->CPUID);
	
	return 0;
}
/*
*********************************************************************************************************
* Description : This function test the software interrupt.
* Notes       : None.
*********************************************************************************************************
*/
uint8_t set_pending(uint8_t argc, char *argv[])
{
    NVIC_SetPendingIRQ(EXTI0_IRQn);
	return 0;
}
/*
*********************************************************************************************************
* Description : This function test the timer of the RTOS.
* Notes       : None.
*********************************************************************************************************
*/
uint8_t timer_manage(uint8_t argc, char *argv[])
{
	extern TimerHandle_t xTimer1;
	extern TimerHandle_t xTimer2;
	
	if(!strcmp(argv[1], "del")){
		if(!strcmp(argv[2], "1")){
			xTimerDelete( xTimer1, 50 );
		}else if(!strcmp(argv[2], "2")){
			xTimerDelete( xTimer1, 50 );
		}else{
			goto para_error;
		}
	}else if(!strcmp(argv[1], "reset")){
		if(!strcmp(argv[2], "1")){	
			/* Reset the software timer.  If the backlight was previously off, then this call  
			   will start the timer.  If the backlight was previously on, then this call will  
			   restart the timer.  A real application may read key presses in an interrupt.  If  
			   this function was an interrupt service routine then xTimerResetFromISR() must be  
			   used instead of xTimerReset(). */ 
			xTimerReset( xTimer1, 50 );
		}else{
			goto para_error;
		}
	}else{
		goto para_error;
	}
	
	return 0;
	
para_error:
	printf("\r\nparameter error.");
	
	return 1;
}
/*
*********************************************************************************************************
* Description : This function ma the task of the RTOS.
* Notes       : None.
*********************************************************************************************************
*/
uint8_t task_manage(uint8_t argc, char *argv[])
{
	extern xTaskHandle xHandle_task1;
	extern xTaskHandle xHandle_task2;
	
	if(!strcmp(argv[1], "1")){
		vTaskDelete( xHandle_task1 );
	}else if(!strcmp(argv[1], "2")){
		vTaskDelete( xHandle_task2 );
	}else{
		printf("\r\nparameter error.");
	}
	
	return 0;
}
