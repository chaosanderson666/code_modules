/*
*********************************************************************************************************
*                                             Command line
*
*                            (c) Copyright 1999, John , All Rights Reserved
*
* File name   : cli.c
* Programmer  : John
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/
#include "stm32f10x.h"
#include "stm32_eval.h"
#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
/* Blocks includes. */
#include "clock.h"
#include "cli.h"
/* debug includes.  */
#include "debug.h"
/* Private macro -------------------------------------------------------------*/
#define mainStackDepth     512
/* Private variables ---------------------------------------------------------*/
xTaskHandle xHandle_task1;
xTaskHandle xHandle_task2;

TimerHandle_t xTimer2 = NULL;
TimerHandle_t xTimer1 = NULL;

QueueHandle_t xQueue_exti0 = NULL;
QueueHandle_t xQueue_uart  = NULL;
/* Private function prototypes -----------------------------------------------*/
/* vTask1 functions ---------------------------------------------------------*/
//__attribute__((section("RAMCODE")))
void vTask1(void *pvParameters )
{
    int8_t c;
	BaseType_t xResult;
    pvParameters = pvParameters;            /* Avoid compiler warning */
	
	while(1){
		xResult = xQueueReceive(xQueue_uart, (void *)&c, pdMS_TO_TICKS(500));
		if(xResult == pdPASS){
    		cmd_line(c);
		}else{
			//printf("\r\nTask1 timeout.");
		}
	}
}
/* vTask2 functions ---------------------------------------------------------*/
void  vTask2(void *pvParameters)
{
	BaseType_t xResult;
	uint32_t EXTI0_Count;
    pvParameters = pvParameters;            /* Avoid compiler warning */
	
    for(;;){
		xResult = xQueueReceive(xQueue_exti0, (void *)&EXTI0_Count, (TickType_t)portMAX_DELAY);
		if(xResult == pdPASS){
			taskENTER_CRITICAL();
			debug_printf("\r\nEXTI0_Count = %d", EXTI0_Count);
			taskEXIT_CRITICAL();
		}
    }
}
/* vTask3 functions ---------------------------------------------------------*/
void  vTask3(void *pvParameters)
{
    pvParameters = pvParameters;            /* Avoid compiler warning */
	
    for(;;){
		vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/* Timer callback function ---------------------------------------------------------*/
static void vTimerCallback( TimerHandle_t pxTimer )
{
	uint32_t ulTimerID;
	
	configASSERT(pxTimer);
	ulTimerID = (uint32_t)pvTimerGetTimerID(pxTimer);
	
	if( 0 == ulTimerID ){
		STM_EVAL_LEDToggle(LED1);
		clk_update();
	}
	if( 1 == ulTimerID ){
		STM_EVAL_LEDToggle(LED2);
	}
}
/* Timer create function ---------------------------------------------------------*/
static void TimerCreate(void)
{
	xTimer1 = xTimerCreate("timer1", pdMS_TO_TICKS(5000), pdTRUE, (void *)0, vTimerCallback);
	xTimer2 = xTimerCreate("timer2", pdMS_TO_TICKS(1000), pdTRUE, (void *)1, vTimerCallback);
	if((xTimer1 != NULL) && (xTimer2 != NULL)){
		/* Start the software timers, using a block time of 0 (no block time).  The scheduler has
		   not been started yet so any block time specified here would be ignored anyway. */
		if(xTimerStart(xTimer1, 0) != pdPASS){
		}
		/* The implementation of xTimerStart() uses the timer command queue, and xTimerStart()  
		   will fail if the timer command queue gets full.  The timer service task does not get  
		   created until the scheduler is started, so all commands sent to the command queue will  
		   stay in the queue until after the scheduler has been started.  Check both calls to  
		   xTimerStart() passed. */
		if(xTimerStart(xTimer2, 0) != pdPASS){
		}
	}
}

/* Queue create function ---------------------------------------------------------*/
void QueueCreate(void)
{	
	UBaseType_t uxQueueLength = 20;
	
	xQueue_exti0 = xQueueCreate(uxQueueLength, sizeof(uint32_t));
	if( xQueue_exti0 == NULL ){
	}
	xQueue_uart = xQueueCreate(uxQueueLength, sizeof(uint8_t));
	if( xQueue_uart == NULL ){
	}
}
/* OS start function ---------------------------------------------------------*/
void OS_Start(void)
{
	TimerCreate();
	QueueCreate();
	xTaskCreate(vTask1, "Console", mainStackDepth, NULL, 4, &xHandle_task1);
	xTaskCreate(vTask2, "Queue",   mainStackDepth, NULL, 3, &xHandle_task2);
	xTaskCreate(vTask3, "vTask3",  mainStackDepth, NULL, 2, &xHandle_task2);
	printf("\r\nstart system.");
	vTaskStartScheduler();
}
/* OS Print function ---------------------------------------------------------*/
void vPrintString( const char *pcString ) 
{ 
    /* Write the string to stdout, using a critical section as a crude method of  
    mutual exclusion. */ 
    taskENTER_CRITICAL();
    { 
        printf( "%s", pcString ); 
        fflush( stdout ); 
    } 
    taskEXIT_CRITICAL();
}
/* This function must be defined in a C source file, not the FreeRTOSConfig.h header 
file. */ 
void vAssertCalled( const char *pcFile, uint32_t ulLine ) 
{ 
    /* Inside this function, pcFile holds the name of the source file that contains  
    the line that detected the error, and ulLine holds the line number in the source  
    file.  The pcFile and ulLine values can be printed out, or otherwise recorded,  
    before the following infinite loop is entered. */ 
    //RecordErrorInformationHere( pcFile, ulLine ); 
	printf("\r\nAssert called at:\r\n%s: %d", pcFile, ulLine);
	printf("\r\ncompiled date and time: %s %s, standard c: %d", __DATE__, __TIME__, __STDC__);
	printf("\r\nsystem halt.");
    /* Disable interrupts so the tick interrupt stops executing, then sit in a loop   
    so execution does not move past the line that failed the assertion. */ 
    taskDISABLE_INTERRUPTS(); 
    for( ;; ); 
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
	printf("\r\nTask %s stack overflow !", pcTaskName);
}
/*-----------------------------------------------------------*/
