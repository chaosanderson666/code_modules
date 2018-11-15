/*
*********************************************************************************************************
*                                             Command line
*
*                            (c) Copyright 1999, John , All Rights Reserved
*********************************************************************************************************
*/
#include <stdint.h>
#include "memory_1.h"
/*
*********************************************************************************************************
*/
#define MB1_MAX   5
#define MB1_SIZE    16

#define MB2_MAX   10
#define MB2_SIZE   64

#define MB3_MAX   5
#define MB3_SIZE   128
/*
*********************************************************************************************************
*/
static int mb1_flg;
static int mb2_flg;
static int mb3_flg;
static char mb1[MB1_MAX][MB1_SIZE];
static char mb2[MB2_MAX][MB2_SIZE];
static char mb3[MB3_MAX][MB3_SIZE];
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
void *malloc_block(int size)
{
	int i;

	if(size == 0){
		return NULL;
	}
	
	if(size <= MB1_SIZE){
		for(i = 0; i < MB1_MAX; i++){
			if((mb1_flg & (1 << i)) == 0){
				mb1_flg |= (1 << i);
				return (void *)&mb1[i];
			}
		}
	}else if(size <= MB2_SIZE){
		for(i = 0; i < MB2_MAX; i++){
			if((mb2_flg & (1 << i)) == 0){
				mb2_flg |= (1 << i);
				return (void *)&mb2[i];
			}
		}
	}else if(size <= MB3_SIZE){
		for(i = 0; i < MB3_MAX; i++){
			if((mb3_flg & (1 << i)) == 0){
				mb3_flg |= (1 << i);
				return (void *)&mb3[i];
			}
		}
	}

	/* allocte failed. */
	return NULL;
}
/*
*********************************************************************************************************
* Description : This function parse the command and execute it.
* Arguments   : None.
* Returns     : the execute status.
* Notes       : none.
*********************************************************************************************************
*/
void free_block(void *ptr)
{
	int i;
	int offset = 0;
	
	if(ptr == NULL){
		return;
	}
	
	if((ptr >= (void *)&mb1[0][0]) 
	&& (ptr < (void *)(&mb1[0][0] + sizeof(mb1)))){
		for(i = 0; i < MB1_MAX; i++){
			offset += MB1_SIZE;
			if(ptr < (void *)(&mb1[0][0] + offset)){
    		  	mb1_flg &= ~(1 << i);
				return;
			}
		}
	}else if((ptr >= (void *)&mb2[0][0]) 
		  && (ptr < (void *)(&mb2[0][0] + sizeof(mb2)))){
		offset = MB2_SIZE;
		for(i = 0; i < MB2_MAX; i++){	
			offset += MB2_SIZE;
			if(ptr <= (void *)(&mb2[0][0] + offset)){
				mb2_flg &= ~(1 << i);
				return;
			}
		}
	}else if((ptr >= (void *)&mb3[0][0]) 
	      && (ptr < (void *)(&mb3[0][0] + sizeof(mb3)))){
		offset = MB3_SIZE;
		for(i = 0; i < MB3_MAX; i++){
			offset += MB3_SIZE;
			if(ptr < (void *)(&mb3[0][0] + offset)){
	 			mb3_flg &= (~(1 << i));
				return;
			}
		}
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
void memset_block(void *m, int value, int size)
{
	int i;
	
	for(i = 0; i < size; i++){
		((uint8_t *)m)[i] = value;
	}
}
/*
*********************************************************************************************************
*/
