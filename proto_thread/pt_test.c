#include <stdio.h>
#include "pt.h"
#include "pt_sem.h"

static struct pt t1, t2, t3, t4;
int flag = 0;

#define NUM_ITEMS 32
#define BUFSIZE   3

static struct pt_sem mutex, full, empty;

PT_THREAD(producer(struct pt *pt))
{
        static int produced;

        PT_BEGIN(pt);

        for (produced = 0; produced < NUM_ITEMS; ++produced) {
                PT_SEM_WAIT(pt, &full);
                PT_SEM_WAIT(pt, &mutex);
                printf("producer produce iterm: %d!\n", produced);
                PT_SEM_SIGNAL(pt, &mutex);
                PT_SEM_SIGNAL(pt, &empty);
        }
	
        PT_END(pt);
}

PT_THREAD(consumer(struct pt *pt))
{
        static int consumed;

        PT_BEGIN(pt);
	
        for (consumed = 0; consumed < NUM_ITEMS; ++consumed) {
                PT_SEM_WAIT(pt, &empty);
                PT_SEM_WAIT(pt, &mutex);
                printf("consumer consumer iterm: %d!\n", consumed);
                PT_SEM_SIGNAL(pt, &mutex);

                PT_SEM_SIGNAL(pt, &full);
        }
	
        PT_END(pt);
}

PT_THREAD(driver_thread(struct pt *pt))
{
        static struct pt pt_producer, pt_consumer;
        PT_BEGIN(pt);

        PT_SEM_INIT(&empty, 0);
        PT_SEM_INIT(&full, BUFSIZE);
        PT_SEM_INIT(&mutex, 1);
        PT_INIT(&pt_producer);
        PT_INIT(&pt_consumer);
        printf("driver_thread wait child!\n");
        PT_WAIT_THREAD(pt, producer(&pt_producer) & consumer(&pt_consumer));
        printf("driver_thread waited child!\n");
	
        PT_END(pt);
}


PT_THREAD(thread1(struct pt *pt))
{
        PT_BEGIN(pt);
        printf("thread 1 wait flag!\n");
        PT_WAIT_UNTIL(pt, flag == 1);
        printf("thread 1 received flag!\n");
        flag = 0;
        PT_END(pt);
}

PT_THREAD(thread2(struct pt *pt))
{
        PT_BEGIN(pt);
        printf("thread 2 send flag!\n");
        flag = 1;
        PT_YIELD(pt);
        printf("thread 2 after yield!\n");
        PT_END(pt);
}

PT_THREAD(thread3(struct pt *pt))
{
        PT_BEGIN(pt);
        printf("thread 3 is running!\n");
        PT_END(pt);
}

int main(void)
{
        PT_INIT(&t1);
        PT_INIT(&t2);
        PT_INIT(&t3);
        PT_INIT(&t4);

        int t = 20;
        while (t--) {
                thread1(&t1);
                thread2(&t2);
                thread3(&t3);
                driver_thread(&t4);
                sleep(1);
        }
}
