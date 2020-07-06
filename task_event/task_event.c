#include "task_event.h"

#define MAX_NUM_OF_TASKS   (5)

struct task {
	unsigned char event; /* max 8 evnets */
	task_handler handler;
	void *paras[8];
};

static struct task tasks[MAX_NUM_OF_TASKS];
static char cur_task_index;

void tasks_run(void)
{
	char i = 0;
	
	while (1) {
		if (tasks[i].handler && tasks[i].event) {
			cur_task_index = i;
			tasks[i].handler(tasks[i].event, tasks[i].paras);
		}
		
		i++;
		if (i > MAX_NUM_OF_TASKS - 1) {
			i = 0;
		}
	}
}

char task_create(task_handler handler)
{
	char index = 0;
	
	enter_critical();
	
	for (index = 0; index < MAX_NUM_OF_TASKS; index++) {
		if (!tasks[index].handler) {
			break;
		}
	}
	
	if (index < MAX_NUM_OF_TASKS) {
		tasks[index].handler = handler;
		tasks[index].event = 0;
	} else {
		index = -1;
	}
	
	exit_critical();
	
	return index;
}

char task_delete(char task_index)
{
	if (task_index > MAX_NUM_OF_TASKS - 1) {
		return -1;
	}
	
	enter_critical();
	tasks[task_index].handler = NULL;
	tasks[task_index].event = 0;
	exit_critical();
	
	return 0;
}

char task_get_task_index(void)
{	
	return cur_task_index;
}

static char task_get_para_index_by_event(char index, unsigned char event)
{
	for (i = 0; i < TASK_EVENT_NUM; i++) {
		if (event == 1 << i) {
			return i;
		}
	}
}

static void *task_set_para_by_event(char index, unsigned char event, void *para)
{
	tasks[index].paras[task_get_para_index_by_event(event)] = para;
}

void *task_get_para_by_event(unsigned char event)
{
	return tasks[cur_task_index].paras[task_get_para_index_by_event(event)];
}

char task_send_event_isr(char task_index, unsigned char event, void *para)
{
	if (task_index > MAX_NUM_OF_TASKS - 1) {
		return -1;
	}
	
	tasks[task_index].event |= event;
	tasks[task_index].paras[task_get_para_index_by_event(event)] = para;
	
	return 0;
}

char task_send_event(char task_index, unsigned char event, void *para)
{
	char ret;
	
	enter_critical();
	ret = task_send_event_isr(task_index, event, para);
	exit_critical();
	
	return ret;
}

