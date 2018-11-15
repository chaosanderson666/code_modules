/*
*********************************************************************************************************
*                                            Delta Timer Manager
*
*                            (c) Copyright 1999, John, All Rights Reserved
*
*********************************************************************************************************
*/
#ifndef __TIMER_DELTA_H__
#define __TIMER_DELTA_H__
/*
*********************************************************************************************************
*/
#include <stdint.h>
#include <stdbool.h>
/*
*********************************************************************************************************
*/
typedef struct timer {                        /* TIMER DATA STRUCTURE                               */
	struct timer *next;
    void (*timer_func)(void *);               /* Function to execute when timer times out           */
    void  *timer_func_arg;                    /* Arguments supplied to user defined function        */
	uint32_t ticks;                           /* Current value of timer ticks                       */
	uint32_t ticks_init;
	uint16_t peri;
} timer_t;
/*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*/
void timer_init(void);
void timer_tick(void);
void timer_task(void);
void timer_set(timer_t *timer, uint32_t ticks, uint16_t peri, void (*func)(void *), void  *func_arg);
void timer_insert(timer_t *new);

uint32_t timer_num(void);


#endif /* __TIMER_DELTA_H__ */
/*
*********************************************************************************************************
*/
