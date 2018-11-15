/*
*********************************************************************************************************
*                                             Qsort Test
*
*                            (c) Copyright 1999, John , All Rights Reserved
*********************************************************************************************************
*/
#include <stdio.h>
#include <stdlib.h>
/*
*********************************************************************************************************
*/
#define LENGTH  256
static int data[LENGTH] = {0};
/*
*********************************************************************************************************
* Description : This function parse the command and execute it.
* Arguments   : None.
* Returns     : the execute status.
* Notes       : none.
*********************************************************************************************************
*/
int comp_inc(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}

/*
*********************************************************************************************************
* Description : This function parse the command and execute it.
* Arguments   : None.
* Returns     : the execute status.
* Notes       : none.
*********************************************************************************************************
*/
int comp_dec(const void *a, const void *b)
{
	return *(int *)b - *(int *)a;
}

/*
*********************************************************************************************************
* Description : This function parse the command and execute it.
* Arguments   : None.
* Returns     : the execute status.
* Notes       : none.
*********************************************************************************************************
*/
void qsort_test(void)
{
	for(int i = 0; i< LENGTH; i++)
	{
		data[i] = rand() % 500;
	}

	printf("increase: ");
	qsort(data, LENGTH, sizeof(data[0]), comp_inc);

	for(int i = 0; i < LENGTH; i++)
	{
		printf("%d ", data[i]);
	}
	printf("\r\n");


	printf("decrease: ");
	qsort(data, LENGTH, sizeof(data[0]), comp_dec);

	for(int i = 0; i < LENGTH; i++)
	{
		printf("%d ", data[i]);
	}
	printf("\r\n");
}
