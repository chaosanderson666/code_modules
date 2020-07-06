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
}
typedef void (*task_handler)(unsigned char event);

static inline void enter_critical(void)
{
	/* disable interrupt */
}

static inline void exit_critical(void)
{
	/* enable interrupt */
}

void tasks_run(void);
char task_create(task_handler handler);
char task_delete(char task_index);
char task_get_task_index(void);

char task_send_event(unsigned char task_index, unsigned char event, void *paras);
char task_send_event_isr(unsigned char task_index, unsigned char event, void *paras);

#endif

