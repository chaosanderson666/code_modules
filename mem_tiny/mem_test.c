#include <stdio.h>
#include <string.h>
#include "mem_tiny.h"

char  *mem_ptr[512];

int main(void)
{
        int i = 0;
        char *new_mem = NULL;

        for (i = 0; ; i++) {
                mem_ptr[i] = (char *)malloc_tiny(10);
                if (!mem_ptr[i]) {
                        printf("out of memory!\n");
                        break;
                }
        }

        for (i = 0; i < 512; i++) {
                free_tiny((void *)(mem_ptr[i]));
        }

	return 0;
}
