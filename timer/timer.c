/*
*********************************************************************************************************
*/
#include <stdbool.h>
#include "timer.h"
/*
*********************************************************************************************************
*/
#define  TMR_MAX_TMR_NUM     5   /* Maximum number of timers that can be created */
/*
*********************************************************************************************************
*/
typedef struct timer {
        void (*timer_func)(void*);                /* Function to execute when timer times out           */
        void  *timer_func_arg;                    /* Arguments supplied to user defined function        */
        uint16_t timer_cur;                       /* Current value of timer ticks                       */
        uint16_t timer_init;                      /* Initial value of timer ticks                       */
        uint8_t timer_peri;                       /* How many times timer can executed                  */
        bool timer_enable;                        /* Flag indicating whether timer is enabled           */
        uint8_t timer_peri_init;                  /* Initial value of timer periodical                  */
} timer_t;
/*
*********************************************************************************************************
*/
static  timer_t timer_table[TMR_MAX_TMR_NUM];   /* Table of timers managed by this module  */
/*
*********************************************************************************************************
* Description : This function initializes the timer manager module.
* Arguments   : None
* Returns     : None.
*********************************************************************************************************
*/
void  timer_init (void)
{
        uint8_t  i;

        /* Clear and disable all timers */
        for (i = 0; i < TMR_MAX_TMR_NUM; i++) {
                timer_table[i].timer_peri = 0;
                timer_table[i].timer_peri_init = 0;
                timer_table[i].timer_enable = false;
                timer_table[i].timer_cur  = 0;
                timer_table[i].timer_init = 0;
                timer_table[i].timer_func = NULL;
                timer_table[i].timer_func_arg = NULL;
        }
}
/*
*********************************************************************************************************
* Description : This function start a timer
* Arguments   : @n   is the timer to start
* Returns     : None.
*********************************************************************************************************
*/
void  timer_start (uint8_t n)
{
        if (n < TMR_MAX_TMR_NUM) {
                timer_table[n].timer_enable = true;
        }
}
/*
*********************************************************************************************************
* Description : This function stops a timer
* Arguments   : @n   is the timer to stop
* Returns     : None.
*********************************************************************************************************
*/
void  timer_stop (uint8_t n)
{
        if (n < TMR_MAX_TMR_NUM) {
                timer_table[n].timer_enable = false;
        }
}
/*
*********************************************************************************************************
* Description : This function clear a timer
* Arguments   : @n   is the timer to stop
* Returns     : None.
*********************************************************************************************************
*/
void  timer_clear (uint8_t n)
{
        if (n < TMR_MAX_TMR_NUM) {
                timer_table[n].timer_enable = false;
                timer_table[n].timer_peri = 0;
                timer_table[n].timer_peri_init = 0;
                timer_table[n].timer_cur  = 0;
                timer_table[n].timer_init = 0;
                timer_table[n].timer_func = NULL;
                timer_table[n].timer_func_arg = NULL;
        }
}
/*
*********************************************************************************************************
* Description : This function checks to see if a timer has expired
* Arguments   : @n      is the timer to check
* Returns     : @0      if the timer has expired
*               @val    the remaining ticks before the timer expires
*********************************************************************************************************
*/
uint16_t  timer_check_ticks (uint8_t n)
{
        uint16_t val;

        val = 0;
        if (n < TMR_MAX_TMR_NUM) {
                val = timer_table[n].timer_cur;
        }
        return (val);
}
/*
*********************************************************************************************************
* Description : This function reloads a timer with its initial ticks
* Arguments   : @n   is the timer to reset
* Returns     : None.
*********************************************************************************************************
*/
void  timer_reset_ticks (uint8_t n)
{
        if (n < TMR_MAX_TMR_NUM) {
                timer_table[n].timer_cur = timer_table[n].timer_init;
        }
}
/*
*********************************************************************************************************
* Description : Set the timer ticks.
* Arguments   : @n      is the timer number 0 ~ TMR_MAX_TMR_NUM-1
*               @ticks  is the number of time to load into the timer
* Returns     : None.
*********************************************************************************************************
*/
void  timer_set_ticks(uint8_t n, uint16_t ticks)
{
        if (n < TMR_MAX_TMR_NUM) {
                timer_table[n].timer_init = ticks;
                timer_table[n].timer_cur  = ticks;
        }
}
/*
*********************************************************************************************************
* Description : Set the timer periodicly.
* Arguments   : @n         is the timer number 0~TMR_MAX_TMR_NUM-1
*               @peri      set the periodic into the timer
* Returns     : None.
*********************************************************************************************************
*/
void  timer_set_peri(uint8_t n, uint8_t peri)
{
        if (n < TMR_MAX_TMR_NUM) {
                timer_table[n].timer_peri = peri;
                timer_table[n].timer_peri_init = peri;
        }
}
/*
*********************************************************************************************************
* Description : Set the timer periodicly.
* Arguments   : @n         is the timer number 0~TMR_MAX_TMR_NUM-1
*               @peri      load initial periodic into the timer
* Returns     : None.
*********************************************************************************************************
*/
void  timer_reset_peri(uint8_t n)
{
        if (n < TMR_MAX_TMR_NUM) {
                timer_table[n].timer_peri = timer_table[n].timer_peri_init;
        }
}
/*
*********************************************************************************************************
* Description : Set the user defined function when the timer expires.
* Arguments   : @n      is the timer number 0 ~ TMR_MAX_TMR_NUM-1
*               @fnct   is a pointer to a function that will be executed when the timer expires
*               @arg    is a pointer to an argument that is passed to 'fnct'
* Returns     : None.
*********************************************************************************************************
*/
void  timer_func_set (uint8_t n, void (*fnct)(void *), void *arg)
{
        if (n < TMR_MAX_TMR_NUM) {
                timer_table[n].timer_func     = fnct;
                timer_table[n].timer_func_arg = arg;
        }
}
/*
*********************************************************************************************************
* Description : Set the timer with the specified arguments.
* Arguments   : @n        is the timer number, can be (0 ~ TMR_MAX_TMR_NUM - 1)
*               @ticks    is the number of ticks to load into the timer, can be (0 ~ 2^16 - 1)
*               @peri     how many times the timer will execute, can be (0 ~ 2^8 - 1), if set to 0,
*                         that means allways execute.
*               @enable   enable or disable the timer, can be (0, 1 or false, true)
*               @fnct     the timer call back function, can be (NULL or else)
*               @arg      the call back function argument
* Returns     : None.
* Note        : ticks and peri must not set to 0 at the same time.
*********************************************************************************************************
*/
void  timer_create(uint8_t n, uint16_t ticks, uint8_t peri, uint8_t enable,
                   void (*fnct)(void *), void *arg)
{
        if (n < TMR_MAX_TMR_NUM) {
                timer_table[n].timer_init = ticks;
                timer_table[n].timer_cur  = ticks;
                timer_table[n].timer_peri = peri;
                timer_table[n].timer_peri_init = peri;
                timer_table[n].timer_func = fnct;
                timer_table[n].timer_func_arg = arg;
                timer_table[n].timer_enable = enable;
        }
}
/*
*********************************************************************************************************
* Description : This task is responsible for updating the timers, it will executes every tick elapses.
* Arguments   : None.
* Returns     : None.
* Note(s)     : None.
*********************************************************************************************************
*/
void timer_task(void)
{
        timer_t  *ptmr;
        uint8_t   i;
        void  (*pfnct)(void *);
        void   *parg;

        /* Start off with no function to execute */
        pfnct = NULL;
        parg  = NULL;

        ptmr = &timer_table[0];
        for (i = 0; i < TMR_MAX_TMR_NUM; i++, ptmr++) {
                /* ENTER_CRITICAL() */
                if (ptmr->timer_enable == true) {
                        /* Decrement timer only if it is enabled */
                        if (ptmr->timer_cur > 0) {
                                ptmr->timer_cur--;
                                if (ptmr->timer_cur == 0) {
                                        if (ptmr->timer_peri == 0) {
						/* forever periodicly */
                                                ptmr->timer_cur = ptmr->timer_init;
                                        } else if (ptmr->timer_peri == 1) {
                                                ptmr->timer_enable = false;
                                        } else if (ptmr->timer_peri > 1) {
                                                ptmr->timer_peri--;
                                                ptmr->timer_cur = ptmr->timer_init;
                                        }
                                        pfnct = ptmr->timer_func;
                                        parg  = ptmr->timer_func_arg;
                                }
                        }
                }
                /* EXIT_CRITICAL() */
                if (pfnct != NULL) {
                        (*pfnct)(parg);   /* ... timed out. */
                }
        }
}
/*
*********************************************************************************************************
*/
