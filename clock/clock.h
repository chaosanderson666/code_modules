/*
*********************************************************************************************************
*                                             Clock/Calendar
*
*                            (c) Copyright 1999, John All Rights Reserved
*
*********************************************************************************************************
*/
#ifndef __CLK_H__
#define __CLK_H__
/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/
#include <stdint.h>
/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void clk_init(void);
void clk_signalclk(void);
void clk_format_time(uint8_t n, char *s);
void clk_format_date(uint8_t n, char *s);
void clk_format_ts(uint8_t n, uint32_t ts, char *s);
void clk_set_time(uint8_t hr, uint8_t min, uint8_t sec);
void clk_set_date(uint8_t month, uint8_t day, uint16_t year);
void clk_set_datetime(uint16_t year, uint8_t month, uint8_t day, uint8_t hr, uint8_t min, uint8_t sec);
void clk_get_datetime(uint8_t *month, uint8_t *day, uint16_t *year, uint8_t *hr, uint8_t *min, uint8_t *sec);
uint32_t clk_get_ts(void);
uint32_t clk_make_ts(uint8_t month, uint8_t day, uint16_t year, uint8_t hr, uint8_t min, uint8_t sec);
void clk_update(void);

#endif /* __CLK_H__ */
/*
*********************************************************************************************************
*/
