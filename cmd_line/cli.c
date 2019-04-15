/*
*********************************************************************************************************
*                                             Command line
*
*                            (c) Copyright 1999, John , All Rights Reserved
*********************************************************************************************************
*/
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "cli.h"
#include "cmds.h"
/*
*********************************************************************************************************
*/
extern int stdarg_printf(const char *format, ...);
#define  cli_printf(format, ...)     stdarg_printf(""format"", ##__VA_ARGS__)
/*
*********************************************************************************************************
*/
#define  MAX_HIS_CMD              10
#define  ARG_MAX_LEN              10
#define  ARG_MAX_NUM              5
#define  CMD_BUF_LEN              (ARG_MAX_NUM * ARG_MAX_LEN)
#define  PROMPT                   ">> "
#define  KEY_RETURN               0x0D
#define  KEY_DELETE               0x7F
#define  KEY_ESC                  0x1B
/*
*********************************************************************************************************
*/
static uint8_t help(uint8_t argc, char *argv[]);
static uint8_t test(uint8_t argc, char *argv[]);
static uint8_t show_his_cmd(uint8_t argc, char *argv[]);
/*
*********************************************************************************************************
*/
typedef struct{
    const char *name;
    uint8_t (*handle)(uint8_t argc, char *argv[ARG_MAX_LEN]);
    const char *help;
}cmd_list_t;
/*
*********************************************************************************************************
*/
static char *argv[ARG_MAX_NUM];
static char rec_buf[CMD_BUF_LEN];
static char his_buf[MAX_HIS_CMD][CMD_BUF_LEN];
static uint8_t rec_charcnt;
static char his_num;
static char his_idx;

static const cmd_list_t cmd_list[] = {
	{ .name = "?",      .handle = help,         .help = "<no param> print the command list and how to use them." },
	{ .name = "c",      .handle = clear_window, .help = "<no param> clear the command window." },
	{ .name = "led",    .handle = led,          .help = "<on/off> turn on or turn off the led." },
	{ .name = "time",   .handle = show_time,    .help = "<none/w> show the current date and time." },
	{ .name = "task",   .handle = show_task,    .help = "<no param> show the task state." },
	{ .name = "reboot", .handle = system_reset, .help = "<no param> reboot the system." },
	{ .name = "mcu",    .handle = dev_infor,    .help = "<no param> show the mcu information." },
	{ .name = "swi",    .handle = set_pending,  .help = "<no param> generate a software interrupt." },
	{ .name = "timer",  .handle = timer_manage, .help = "<timer id> manage the timer of RTOS." },
	{ .name = "kill",   .handle = task_manage,  .help = "<task id> delete the task of RTOS." },
	{ .name = "his",    .handle = show_his_cmd, .help = "<no param> show the history commands." },
    { .name = "test",   .handle = test,         .help = "<anything> test the command line." },
};

/*
*********************************************************************************************************
* Description : This function parse the command and execute it.
* Arguments   : None.
* Returns     : the execute status.
* Notes       : none.
*********************************************************************************************************
*/
static uint8_t cmd_excute(void)
{
	uint8_t argc;
	uint8_t ret;
	uint8_t i;
	uint8_t cmd_num;
	char pre_cmd_idx;

	/* if it is NUL, do nothing. */
	if(rec_buf[0] == 0x00){  
		return 0;
	}
	
	if(his_idx > 0){
		/* get the previous command index. */
		pre_cmd_idx = his_idx - 1;
	}
	
	/* if the new command is the same as previous one, 
	 * then don't save it.
	 */
	if(strcmp(&his_buf[pre_cmd_idx][0], rec_buf) != 0){
		if(his_num == MAX_HIS_CMD){
			for(i = 0; i < MAX_HIS_CMD - 1; i++){
				memcpy(&his_buf[i][0], &his_buf[i + 1][0], CMD_BUF_LEN);
			}
			memset(&his_buf[i][0], 0, CMD_BUF_LEN);
			memcpy(&his_buf[i][0], rec_buf, CMD_BUF_LEN);
		}else{
			memset(&his_buf[his_num][0], 0, CMD_BUF_LEN);
			memcpy(&his_buf[his_num][0], rec_buf, CMD_BUF_LEN);
			his_num++;
		}
		his_idx = his_num;
	}

	argv[0] = strtok(rec_buf, " \t");
    
	for(i = 1; i <= ARG_MAX_NUM; i++){
		argv[i] = strtok(NULL, " \t");
		if(NULL == argv[i])
			break;
	}
	
    if(i > ARG_MAX_NUM){
        return cli_printf("\r\ntoo many argument !");
    }

    argc = i;

    cmd_num = sizeof(cmd_list)/sizeof(cmd_list[0]);
    for(i = 0; i < cmd_num; i++){
        if(strcmp(argv[0], cmd_list[i].name) == 0){
            ret = cmd_list[i].handle(argc, argv);
            break;
        }
    }

    if(i >= cmd_num){
        cli_printf("\r\n Command \'%s\' unknown, type ? for help.", rec_buf);
    }

    memset(rec_buf, 0, sizeof(rec_buf));
	rec_charcnt = 0;
	
    return ret;
}
/*
*********************************************************************************************************
* Description : This function get a char from UART.
* Arguments   : c.
* Returns     : None.
* Notes       : - This function should called by UART interrupt.
*********************************************************************************************************
*/
static void return_key_evt(void)
{
	cmd_excute();
	cli_printf("\r\n%s", PROMPT);
}
/*
*********************************************************************************************************
* Description : This function get a char from UART.
* Arguments   : c.
* Returns     : None.
* Notes       : - This function should called by UART interrupt.
*********************************************************************************************************
*/
static void del_key_evt(void)
{
	if(rec_charcnt > 0){
		rec_charcnt--;
		rec_buf[rec_charcnt] = 0;
		cli_printf("%c%c%c", '\b', ' ', '\b');   /* echo backspace character. */
	}
}
/*
*********************************************************************************************************
* Description : This function get a char from UART.
* Arguments   : c.
* Returns     : None.
* Notes       : - This function should called by UART interrupt.
*********************************************************************************************************
*/
static void up_key_evt(void)
{
	uint8_t i;
	
	if(his_idx > 0){
		his_idx --;
	}else{
		return;
	}

	for(i = 0; i < rec_charcnt; i++){
		cli_printf("%c%c%c", '\b', ' ', '\b');
	}

	memset(rec_buf, 0, CMD_BUF_LEN);
	memcpy(rec_buf, &his_buf[his_idx][0], CMD_BUF_LEN);
	rec_charcnt = strlen(rec_buf);
	cli_printf("%s", rec_buf);
}
/*
*********************************************************************************************************
* Description : This function get a char from UART.
* Arguments   : c.
* Returns     : None.
* Notes       : - This function should called by UART interrupt.
*********************************************************************************************************
*/
static void down_key_evt(void)
{
	uint8_t i;
	
	if(his_idx < MAX_HIS_CMD){
		his_idx++;
	} else {
		return;
	}
	
	for(i = 0; i < rec_charcnt; i++){
		cli_printf("%c%c%c", '\b', ' ', '\b');
	}
	memset(rec_buf, 0, CMD_BUF_LEN);
	memcpy(rec_buf, &his_buf[his_idx][0], CMD_BUF_LEN);
	rec_charcnt = strlen(rec_buf);
	cli_printf("%s", rec_buf);
}
/*
*********************************************************************************************************
* Description : This function get a char from UART.
* Arguments   : c.
* Returns     : None.
* Notes       : None.
*********************************************************************************************************
*/
void cmd_line(char c)
{
	static bool esc = false;
	static bool lsb = false;  /* left square bracket. */
	
	switch(c)
	{
		case KEY_RETURN : return_key_evt(); return; /* return key. */
		case KEY_DELETE : del_key_evt();    return; /* delete key. */
		case KEY_ESC    : esc = true;       return;
		default : {
			if(esc == true){
				esc = false;
				if(c == '['){
					lsb = true;
					return;
				}
			}
			
			if(lsb == true){
				lsb = false;
				switch(c){
					case 0x41 : up_key_evt();   return;  /* up key.  */
					case 0x42 : down_key_evt(); return;  /* down key.  */ 
					case 0x43 : return;                  /* right key. */
					case 0x44 : return;                  /* left key.  */
					default   : return;
				}
			}
			
			/* Filter the invalid character. ' ' - '~' */
			if(c < 0x20 || c > 0x7e){
				return;
			}
			
			cli_printf("%c", c);    /* echo character. */
			rec_buf[rec_charcnt] = c;
			rec_charcnt++;

			if(rec_charcnt >= CMD_BUF_LEN){		
                cli_printf("\r\n command too long.\r\n%s", PROMPT);
                memset(rec_buf, 0, sizeof(rec_buf));
				rec_charcnt = 0;
			}
		}
	}
}

/*
*********************************************************************************************************
* Description : This function print the commands help information.
* Arguments   : argc.
* Returns     : argv.
* Notes       : none.
*********************************************************************************************************
*/
static uint8_t help(uint8_t argc, char *argv[])
{
	uint8_t i;
	
    for(i = 0; i < sizeof(cmd_list)/sizeof(cmd_list[0]); i++){
        cli_printf("\r\n   %-6s  %-s", cmd_list[i].name, cmd_list[i].help);
    }
    
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
static uint8_t test(uint8_t argc, char *argv[])
{
	uint8_t i;
	
	for(i = 0; i < argc; i++){
		cli_printf("\r\n%s", argv[i]);
	}
	return 0;
}
/*
*********************************************************************************************************
* Description : This function just used to show the history command.
* Arguments   : argc.
* Returns     : argv.
* Notes       : None.
*********************************************************************************************************
*/
static uint8_t show_his_cmd(uint8_t argc, char *argv[])
{
	uint8_t i;
	
	for(i = 0; i < his_num; i++){
		cli_printf("\r\n %s", &his_buf[i][0]);
	}
	return 0;
}
