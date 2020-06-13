#include "task_event.h"

typedef (void)(*task_handler)(char, void *);

struct task {
	const char *name;
	unsigned char event;
	const task_handler handler;
	void *paras;
};

extern void task0_handler(char event, void *paras);
extern void task1_handler(char event, void *paras);
extern void task2_handler(char event, void *paras);

struct task tasks[] = {
	[TASK_INDEX_0] = {
		.name = "task0",
		.event = 0,
		.handler = task0_handler,
		.paras = NULL,
	},
	
	[TASK_INDEX_1] = {
		.name = "task1",
		.event = 0,
		.handler = task1_handler,
		.paras = NULL,
	},
	
	[TASK_INDEX_2] = {
		.name = "task1",
		.event = 0,
		.handler = task1_handler,
		.paras = NULL,
	},
};

void tasks_run(void)
{
	char i = 0;
	
	while (1) {
		if (tasks[i].event) {
			tasks[i].handler(tasks[i].event, tasks[i].paras);
			tasks[i].event = 0;
			tasks[i].paras = NULL;
		}
		
		if (i++ >= sizeof(tasks)/sizeof(tasks[0])) {
			i = 0;
		}
	}
}

char task_send_event(int task_index, unsigned char event, void *paras)
{
	if (task_index >= sizeof(tasks)/sizeof(tasks[0])) {
		return -1;
	}
	
	tasks[task_index].event |= event;
	tasks[task_index].paras = paras;
}

