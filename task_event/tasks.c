#include "task_event.h"

void task0_handler(char event, void *paras)
{
	int cnt =  *(int *)paras;

	if (event & TASK_EVENT_0) {
		return;
	}

	if (event & TASK_EVENT_1) {
		return;
	}

	if (event & TASK_EVENT_2) {
		return;
	}
}

void task1_handler(char event, void *paras)
{
	int cnt =  *(int *)paras;

	if (event & TASK_EVENT_0) {
		return;
	}

	if (event & TASK_EVENT_1) {
		return;
	}

	if (event & TASK_EVENT_2) {
		return;
	}
}

void task3_handler(char event, void *paras)
{
	int cnt =  *(int *)paras;

	if (event & TASK_EVENT_0) {
		return;
	}

	if (event & TASK_EVENT_1) {
		return;
	}

	if (event & TASK_EVENT_2) {
		return;
	}
}

void main(void)
{
	tasks_run();
}

int interrupt_test(void)
{
	task_send_event(TASK_INDEX_0, TASK_EVENT_0, NULL);
}


