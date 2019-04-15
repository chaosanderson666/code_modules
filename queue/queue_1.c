/*
*********************************************************************************************************
*                                              queue_1.c
*
*                            (c) Copyright 1999, John All Rights Reserved
*
*********************************************************************************************************
*/
#include "queue_1.h"
#include "memory_1.h"
/*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Description : This function parse the command and execute it.
* Arguments   : None.
* Returns     : the execute status.
* Notes       : none.
*********************************************************************************************************
*/
void queue_init(queue_h_t *head, uint8_t max_cnt, uint8_t iterm_size)
{
	head->max = max_cnt;
	head->cnt = 0;
	head->iterm_size = iterm_size;
	INIT_LIST_HEAD(&head->list);
}
/*
*********************************************************************************************************
* Description : This function parse the command and execute it.
* Arguments   : None.
* Returns     : the execute status.
* Notes       : none.
*********************************************************************************************************
*/
void queue_send(queue_h_t *head, void *data)
{
	queue_t *new_node;
	
	if(head->cnt < head->max){
		new_node = (queue_t *)malloc_1(sizeof(queue_t) + head->iterm_size);
		//memcpy_1(new_node->data, data, head->iterm_size);
		list_add(&new_node->list, &head->list);
		head->cnt++;
	}
}
/*
*********************************************************************************************************
* Description : This function parse the command and execute it.
* Arguments   : None.
* Returns     : the execute status.
* Notes       : none.
*********************************************************************************************************
*/
queue_t *queue_receive(queue_h_t *head)
{
	queue_t *node;
	struct list_head *pos;
	
	if(list_empty(&head->list) == 1){
		return NULL;
	}
	
	node = list_entry(pos, queue_t, list);
	list_del(&node->list);
	head->cnt--;
	
	return node->data;
}
/*
*********************************************************************************************************
*/








