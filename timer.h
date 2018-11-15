/*
*********************************************************************************************************
*                                             Timer Manager
*
*                            (c) Copyright 1999, John, All Rights Reserved
*
*********************************************************************************************************
*/
#ifndef __TIMER_H__
#define __TIEMR_H__
/*
*********************************************************************************************************
*/
#include <stdint.h>
#include <stdbool.h>
/*
*********************************************************************************************************
*/
#define  TMR_MAX_TMR_NUM     1   /* Maximum number of timers that can be created */
/*
*********************************************************************************************************
*/
void  timer_init(void);
void  timer_task(void);
void  timer_tick(void);

void  timer_stop(uint8_t n);
void  timer_start(uint8_t n);
void  timer_clear(uint8_t n);

void  timer_reset_peri(uint8_t n);
void  timer_set_peri(uint8_t n, uint8_t peri);

void  timer_reset_ticks(uint8_t n);
void  timer_set_ticks(uint8_t n, uint16_t ticks);

void  timer_func_set(uint8_t n, void (*fnct)(void *), void *arg);

void  timer_create(uint8_t n, uint16_t ticks, uint8_t peri, uint8_t enable, 
						void (*fnct)(void *), void *arg);

uint16_t timer_check_ticks(uint8_t n);



#endif /* __TIEMR_H__ */
/*
*********************************************************************************************************
*/
