/*
*********************************************************************************************************
*                                            Debug
*                            (c) Copyright 1990, John , All Rights Reserved
*
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*/
#include <stdio.h>
/*
*********************************************************************************************************
*/
#ifdef DEBUG
	#define debug_printf(format, ...) printf("\r\n"__FILE__", %d: "format"", __LINE__, ##__VA_ARGS__)
#else
	#define debug_printf(format, ...)
#endif    /* __DEBUG__ */
