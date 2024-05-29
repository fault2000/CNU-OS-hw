#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#define THREAD_STACKSIZE 1024

// task status
typedef enum {
	TASK_READY = 0,
	TASK_RUN,
	TASK_YIELD,
	TASK_SLEEP,
	TASK_KILL
} TaskStatus;

typedef struct task_info_tag {
	unsigned long stack[THREAD_STACKSIZE];
	unsigned long sp;

	int task_id;
	TaskStatus status;

	struct task_info_tag *prev;
	struct task_info_tag *next;
}* TaskInfo;

typedef void (*TaskFunc)(void *context);

TaskInfo thread_create(TaskFunc callback, void *context);

void thread_init(void);
void thread_wait(void);
void thread_uninit(void);
void thread_switch(int);
void thread_kill(void);

#endif
