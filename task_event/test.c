#include <stdio.h>
#include "task_event.h"

char task0_index;
char task1_index;
char task2_index;

void task0_handler(unsigned char event)
{
        printf("\r\ntask %d event %d", task_get_task_index(), event);

        if (event & TASK_EVENT_0) {
                task_event_send(task1_index, TASK_EVENT_0, NULL);
                task_event_clear(TASK_EVENT_0);
                return;
        }

        if (event & TASK_EVENT_1) {
                task_event_send(task1_index, TASK_EVENT_1, NULL);
                task_event_clear(TASK_EVENT_1);
                return;
        }
}

void task1_handler(unsigned char event)
{
        printf("\r\ntask %d event %d", task_get_task_index(), event);

        if (event & TASK_EVENT_0) {
                task_event_send(task2_index, TASK_EVENT_0, NULL);
                task_event_clear(TASK_EVENT_0);
                return;
        }
        if (event & TASK_EVENT_1) {
                task_event_send(task2_index, TASK_EVENT_1, NULL);
                task_event_clear(TASK_EVENT_1);
                return;
        }
}

void task2_handler(unsigned char event)
{
        printf("\r\ntask %d event %d", task_get_task_index(), event);

        if (event & TASK_EVENT_0) {
                task_event_send(task0_index, TASK_EVENT_1, NULL);
                task_event_clear(TASK_EVENT_0);
                return;
        }
        if (event & TASK_EVENT_1) {
                task_event_send(task0_index, TASK_EVENT_0, NULL);
                task_event_clear(TASK_EVENT_1);
                return;
        }
}

void main(void)
{

        task0_index = task_create(task0_handler);
        if (task0_index < 0) {
                return;
        }

        task1_index = task_create(task1_handler);
        if (task1_index < 0) {
                return;
        }

        task2_index = task_create(task2_handler);
        if (task2_index < 0) {
                return;
        }

        task_event_send(task0_index, TASK_EVENT_0, NULL);

        tasks_run();
}

