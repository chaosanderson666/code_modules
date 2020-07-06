#include "task_event.h"
#include "user_tasks.h"

char task0_index;
char task1_index;
char task2_index;

void task0_handler(unsigned char event)
{
	void *para = NULL;

	if (event & TASK0_EVT_START) {
		para = task_get_para_by_event(TASK0_EVT_START);
	}

	if (event & TASK0_EVT_STOP) {
		para = task_get_para_by_event(TASK0_EVT_STOP);
	}
}

void task1_handler(unsigned char event)
{
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

void task2_handler(unsigned char event)
{
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

void user_create_tasks(void)
{

	task0_index = task_create(task0_handler);
	task1_index = task_create(task1_handler);
	task2_index = task_create(task2_handler);

	tasks_run();
}

int send_event_task0(unsigned char event)
{
	task_send_event(task0_index, TASK_EVENT_0, NULL);
}


