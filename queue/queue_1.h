/*
*********************************************************************************************************
*                                              queue.h
*
*                            (c) Copyright 1999, John All Rights Reserved
*
*********************************************************************************************************
*/
#ifndef _QUEUE_H_
#define _QUEUE_H_
/*
*********************************************************************************************************
*/
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "dc_list.h"
/*
*********************************************************************************************************
*/
typedef struct queue_n{
	void *data;
	struct list_head list;
}queue_t;
/*
*********************************************************************************************************
*/
typedef struct queue_h{
	uint8_t max;
	uint8_t cnt;
	uint8_t iterm_size;
	struct list_head list;
}queue_h_t;
/*
*********************************************************************************************************
*/
void queue_init(queue_h_t *head, uint8_t max_cnt, uint8_t iterm_size);
void queue_send(queue_h_t *head, void *data);
queue_t *queue_receive(queue_h_t *head);
/*
*********************************************************************************************************
*/
#endif /* _QUEUE_H_ */
/*
*********************************************************************************************************
*/