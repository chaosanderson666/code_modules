/*
*********************************************************************************************************
*                                             Command Function
*
*                            (c) Copyright 1999, John , All Rights Reserved
*
* File name   : cmds.h
* Programmer  : John
*********************************************************************************************************
*/
#ifndef __CMDS_H_
#define __CMDS_H_
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
uint8_t          led(uint8_t argc, char *argv[]);
uint8_t    show_time(uint8_t argc, char *argv[]);
uint8_t clear_window(uint8_t argc, char *argv[]);
uint8_t    show_task(uint8_t argc, char *argv[]);
uint8_t system_reset(uint8_t argc, char *argv[]);
uint8_t    dev_infor(uint8_t argc, char *argv[]);
uint8_t  set_pending(uint8_t argc, char *argv[]);
uint8_t timer_manage(uint8_t argc, char *argv[]);
uint8_t  task_manage(uint8_t argc, char *argv[]);
#endif  /* __CMDS_H_ */

