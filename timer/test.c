#include <stdio.h>
#include <string.h>

#include "timer.h"


void timer_cb(void *arg)
{
    printf("===========\n");
    timer_stop(0);
}

int main(void)
{

    timer_create(0, 10000, 0, 1, timer_cb, NULL);

    while(1) {
          printf("-------------\n");
          timer_task();
          sleep(1);
    }
    return 0;
}


