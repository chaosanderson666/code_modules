#ifndef __TASK_EVENT_H__
#define __TASK_EVENT_H__

enum {
	TASK_EVENT_0 = 1 << 0,
	TASK_EVENT_1 = 1 << 1,
	TASK_EVENT_2 = 1 << 2,
	TASK_EVENT_3 = 1 << 3,
	TASK_EVENT_4 = 1 << 4,
	TASK_EVENT_5 = 1 << 5,
	TASK_EVENT_6 = 1 << 6,
	TASK_EVENT_7 = 1 << 7,
	TASK_EVENT_NUM = 8,
};

typedef void (*task_handler)(unsigned char event);

void enter_critical(void);
void exit_critical(void);

void tasks_run(void);
char task_create(task_handler handler);
char task_delete(char task_index);
char task_get_task_index(void);

char task_event_send(char task_index, unsigned char event, void *para);
char task_event_send_isr(char task_index, unsigned char event, void *para);
char task_event_clear(unsigned char event);

#endif

