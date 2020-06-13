#ifndef __TASK_EVENT_H__
#define __TASK_EVENT_H__

#define TASK_INDEX_0   (0)
#define TASK_INDEX_1   (1)
#define TASK_INDEX_2   (2)

#define TASK_EVENT_0   (1 << 0)
#define TASK_EVENT_1   (1 << 1)
#define TASK_EVENT_2   (1 << 2)
#define TASK_EVENT_3   (1 << 3)
#define TASK_EVENT_4   (1 << 4)

void tasks_run(void);
char task_send_event(int task_index, unsigned char event, void *paras);

#endif

