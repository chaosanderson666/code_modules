/*
*********************************************************************************************************
*                                              queue test
*
*                            (c) Copyright 1999, John All Rights Reserved
*
*********************************************************************************************************
*/
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "queue_1.h"
#include "memory_1.h"
/*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*/

static queue_h_t q_head;

void queue_test(void)
{
	queue_t *new_node, *received;

	static uint8_t c = 0x1;

	queue_init(&q_head, 5, 1);

	queue_send(&q_head, (void *)&c);
	c++;
	queue_send(&q_head, (void *)&c);
	c++;
	queue_send(&q_head, (void *)&c);
	
	while(1){
		received = queue_receive(&q_head);
		if(received == NULL){
			return;
		}
		printf("%d", (int*)received->data);
	}
	
}
