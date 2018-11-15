/*
*********************************************************************************************************
*                                            Timer Manager
*                           (c) Copyright 1999, John, All Rights Reserved
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*/
#include <stdio.h>
#include <stdint.h>
#include "timer_delta.h"
/*
*********************************************************************************************************
*/
static uint8_t time_up;
static timer_t head;
/*
*********************************************************************************************************
*/
#define timer_list_empty()  ((head.next == NULL) ? true : false)
/*
*********************************************************************************************************
* Description : This function is called by the 'clock tick' ISR on every tick.  This function is thus
*               responsible for counting the number of clock ticks.  When elapses, this function will 
*               signal the timer manager task.
* Arguments   : None.
* Returns     : None.
*********************************************************************************************************
*/
void timer_init(void)
{
	head.next = NULL;
	head.ticks = 0;
	head.ticks_init = 0;
	head.peri = 0;
	head.timer_func = NULL;
	head.timer_func_arg = NULL;
}
/*
*********************************************************************************************************
* Description : This function is called by the 'clock tick' ISR on every tick.  This function is thus
*               responsible for counting the number of clock ticks.  When elapses, this function will 
*               signal the timer manager task.
* Arguments   : None.
* Returns     : None.
*********************************************************************************************************
*/
uint32_t timer_num(void)
{
    return head.ticks;
}
/*
*********************************************************************************************************
* Description : This function is called by the 'clock tick' ISR on every tick.  This function is thus
*               responsible for counting the number of clock ticks.  When elapses, this function will 
*               signal the timer manager task.
* Arguments   : None.
* Returns     : None.
*********************************************************************************************************
*/
void timer_set(timer_t *timer, uint32_t ticks, uint16_t peri, void (*func)(void *), void  *func_arg)
{
	timer->next = NULL;
	timer->ticks = ticks;
	timer->ticks_init = ticks;
	timer->peri = peri;
	timer->timer_func = func;
	timer->timer_func_arg = func_arg;
	timer_insert(timer);
}
/*
*********************************************************************************************************
* Description : This function is called by the 'clock tick' ISR on every tick.  This function is thus
*               responsible for counting the number of clock ticks.  When elapses, this function will 
*               signal the timer manager task.
* Arguments   : None.
* Returns     : None.
*********************************************************************************************************
*/
void timer_insert(timer_t *new)
{
	uint32_t delta_ticks;
	timer_t *node;
	
	node = head.next;
	if( node == NULL ) {
		head.next = new;
		new = NULL;
        head.ticks++;
		return;
	}
	if(new->ticks < node->ticks){
		node->ticks -= new->ticks;
		new->next = head.next;
		head.next = new;
        head.ticks++;
		return;
	} else {
		delta_ticks = new->ticks;
		for(;;){
			delta_ticks = delta_ticks - node->ticks;
			if(node->next == NULL) {
				node->next = new;
				new->next = NULL;
				new->ticks = delta_ticks;
                
                head.ticks++;
				return;
			}
			if(delta_ticks < node->next->ticks){
				node->next->ticks -= delta_ticks;
				new->next = node->next;
				node->next = new;
				new->ticks = delta_ticks;
                
                head.ticks++;
				return;
			}
			node = node->next;
		}
	}
}
/*
*********************************************************************************************************
* Description : This function is called by the 'clock tick' ISR on every tick.  This function is thus
*               responsible for counting the number of clock ticks.  When elapses, this function will 
*               signal the timer manager task.
* Arguments   : None.
* Returns     : None.
*********************************************************************************************************
*/
void  timer_tick (void)
{
    /* if there is operating system, we should use the communication object, like semaphore. */
    time_up = true;
}
/*
*********************************************************************************************************
* Description : This task is created by timer_init() and is responsible for updating the timers.
*               timer_task() executes every 1/10 of a second.
* Arguments   : None.
* Returns     : None.
* Note(s)     : 1) The function to execute when a timer times out is executed outside the critical
*                  section.
*********************************************************************************************************
*/
void timer_task(void)
{
	timer_t *temp;
	
	if (time_up == false) {
		return;
	} else {
		time_up = false;
	}
	
	if(timer_list_empty() == true) {
		return;
	}

	for(; head.next != NULL; ){	
		if(head.next->ticks > 0){
			head.next->ticks--;
			return;
		} else {
			(*(head.next->timer_func))(head.next->timer_func_arg);
			head.ticks--;
			if(head.next->peri == 0){
				head.next->ticks = head.next->ticks_init;
				temp = head.next;
				head.next = head.next->next;
				timer_insert(temp);
			}else if(head.next->peri == 1){
				head.next = head.next->next;
			}else if(head.next->peri > 1){
				head.next->peri--;
				head.next->ticks = head.next->ticks_init;
				temp = head.next;
				head.next = head.next->next;
				timer_insert(temp);
			}
		}
	}
}
/*
*********************************************************************************************************
*/
