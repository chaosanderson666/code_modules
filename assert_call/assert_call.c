/*
*********************************************************************************************************
*                                             Assert Call
*
*                            (c) Copyright 1999, John All Rights Reserved
*
*********************************************************************************************************
*/
#include <stdio.h>
#include "assert_call.h"
/*
*********************************************************************************************************
* Description : None.
* Arguments   : None.
* Returns     : None.
* Notes       : None.
*********************************************************************************************************
*/
void assert_called( const char *src_file_name, uint32_t file_line )
{ 
    /* Inside this function, src_file_name holds the name of the source file that contains  
	   the line that detected the error, and file_line holds the line number in the source  
       file. 
	*/ 
	printf("\r\nAssert failed: %s: %d\r\n", src_file_name, file_line);
	//printf("\r\ncompiled date and time: %s %s, standard c: %d", __DATE__, __TIME__, __STDC__);
	printf("\r\nsystem halt.");
    /* Disable interrupts so the tick interrupt stops executing, then sit in a loop   
	   so execution does not move past the line that failed the assertion.
	*/ 
    //DISABLE_INTERRUPTS();
    for( ;; ); 
}
/*
*********************************************************************************************************
*/
