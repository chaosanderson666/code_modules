/*
*********************************************************************************************************
*                                             Protothread Test
*
*                            (c) Copyright 1999, John All Rights Reserved
*
*********************************************************************************************************
*/
#include <stdio.h>
#include "pt.h"
#include "stm32_eval.h"
#include "cli.h"
#include "clock.h"
/*
*********************************************************************************************************
*/
static struct pt t1, t2, t3;
int flag1 = 0;
//char c;
/*
*********************************************************************************************************
* Description : This function init the protothreads.
* Arguments   : None.
* Returns     : the excute status.
* Notes       : none.
*********************************************************************************************************
*/
void pt_init(void)
{
	PT_INIT(&t1);
	PT_INIT(&t2);
	PT_INIT(&t3);
}
/*
*********************************************************************************************************
* Description : This function parse the command and excute it.
* Arguments   : None.
* Returns     : the excute status.
* Notes       : none.
*********************************************************************************************************
*/
static PT_THREAD(task1(struct pt *pt))
{  
	PT_BEGIN(pt);
	PT_WAIT_UNTIL(pt, flag1 != 0);
	STM_EVAL_LEDToggle(LED1);
	flag1 = 0;
	PT_END(pt);
}
/*
*********************************************************************************************************
* Description : This function parse the command and excute it.
* Arguments   : None.
* Returns     : the excute status.
* Notes       : none.
*********************************************************************************************************
*/
static PT_THREAD(task2(struct pt *pt))
{  
	PT_BEGIN(pt);
	//STM_EVAL_LEDToggle(LED2);
	clk_update();
	PT_YIELD(pt);
	PT_END(pt);
}
/*
*********************************************************************************************************
* Description : This function parse the command and excute it.
* Arguments   : None.
* Returns     : the execute status.
* Notes       : none.
*********************************************************************************************************
*/
static PT_THREAD(task3(struct pt *pt))
{  
	PT_BEGIN(pt);
	//PT_WAIT_UNTIL(pt, c != 0);
	//cmd_line(c);
	//flag1 = 1;
	//c = 0;
	PT_END(pt);
}
/*
*********************************************************************************************************
* Description : 
* Arguments   : None.
* Returns     : None.
* Notes       : none.
*********************************************************************************************************
*/
void pt_start(void)
{
	//int t;
	task1(&t1);
	//t = 20000; while(t--);
	task2(&t2);
	task3(&t3);
	//t = 20000; while(t--);
}
/*
*********************************************************************************************************
*/
