#include <stdio.h>
#include <signal.h>
#include <setjmp.h>

#include <malloc.h>
#include <memory.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

#include "scheduler.h"

// necessary saved data in switching task
typedef struct {
	unsigned long flags;

	unsigned long rbp;

	unsigned long rdi;
	unsigned long rsi;

	unsigned long rdx;
	unsigned long rcx;
	unsigned long rbx;
	unsigned long rax;

	unsigned long callback;
	unsigned long retaddr;
	unsigned long data;
} frame;

typedef struct sch_handle_tag {
	int child_task;

	TaskInfo running_task;
	TaskInfo root_task;
} SchHandle;

// global schedule handler
SchHandle gh_sch;

// task data struct
TaskInfo task_get_running_task(void);
void task_insert(TaskInfo);
void task_delete(TaskInfo);
void task_next(void);
void scheduler(void);
void parent_task(void *);

// thread_create : create task function, allocate and construct task struct
TaskInfo thread_create(TaskFunc callback, void *context) {
	TaskInfo task;

	task = malloc(sizeof(struct task_info_tag));
	memset(task, 0x00, sizeof(struct task_info_tag));

	frame* f = (frame *) &(task->stack[
		THREAD_STACKSIZE - sizeof(frame)
	]);

	for (int i = 0; i < THREAD_STACKSIZE; i++)
		task->stack[i] = i;

	memset(f, 0x00, sizeof(frame));
	f->callback = (unsigned long) callback;
	f->retaddr = (unsigned long) thread_kill;
	f->data = (unsigned long) context;
	
	f->rbp = (unsigned long) &f->rax;

	task->sp = (unsigned long) f;

	// Indicated that the number of child tasks has increased in gh_sch as the task is created
	gh_sch.child_task++;

	// assign task_id to gh_sch.child_task value
	task->task_id = gh_sch.child_task;

	// set status to TASK_READY at task creation
	task->status = TASK_READY;

	// insert task struct to linkedlist of task struct
	task_insert(task);

	return task;
}

// thread_init : initialize function, main function call this first
// 		 initialize global scheduler handler, create parent_task
void thread_init(void) {
	gh_sch.root_task = NULL;
	gh_sch.running_task = NULL;

	gh_sch.child_task = 0;

	thread_create(parent_task, NULL);
}

// thread_switch : function that causes a currently executing task to yield CPU usage to another waiting task
// 		   values of current cpu register are stored in stack section of executing task
// 		   and stack informations of next task are restored in register
static unsigned long spsave, sptmp;
void thread_switch(int unused)
{
	asm(	"push %%rax	\n\t"
		"push %%rbx	\n\t"
		"push %%rcx	\n\t"
		"push %%rdx	\n\t"
		"push %%rsi	\n\t"
		"push %%rdi	\n\t"
		"push %%rbp	\n\t"
		"pushf		\n\t"
		"mov %%rsp, %0	": "=r" (spsave));

	gh_sch.running_task->sp = spsave;

	scheduler();

	sptmp = gh_sch.running_task->sp;

	asm(	"mov %0, %%rsp	\n\t"
		"popf		\n\t"
		"pop %%rbp	\n\t"
		"pop %%rdi	\n\t"
		"pop %%rsi	\n\t"
		"pop %%rdx	\n\t"
		"pop %%rcx	\n\t"
		"pop %%rbx	\n\t"
		"pop %%rax	\n\t"
		::"r" (sptmp));
}

// scheduler : function that select next task
void scheduler(void) {
	/* TODO fill this section */

}

void thread_wait(void) {
	parent_task(NULL);
}

// thread_kill : function that change task status to TASK_KILL
void thread_kill(void) {
	/* TODO fill this section */

}

void thread_uninit(void) {
	return;
}

// parent_task : function that call thread_switch() until no more child thread
void parent_task(void *context) {
	// struct for signal handling
	struct sigaction act;
	sigset_t masksets;
	pid_t pid;

	// initalize signal set
	sigemptyset(&masksets);

	// register thread_switch() to signal handler
	act.sa_handler = thread_switch;
	act.sa_mask = masksets;
	act.sa_flags = SA_NODEFER;

	// set action when signal received
	sigaction(SIGUSR1, &act, NULL);

	/* TODO fill this section */

}

// task_insert : function that insert new taskinfo to linkedlist
void task_insert(TaskInfo task) {
	/* TODO fill this section */

}

// task_get_running_task : function that return task pointed gh_sch.running_task in linkedlist
TaskInfo task_get_running_task(void) {
	/* TODO fill this section */

}

// task_next : function that return next task pointed gh_sch.running_task in linkedlist
void task_next(void) {
	/* TODO fill this section */

}

// task_delete : function that delete task in linkedlist
void task_delete(TaskInfo task) {
	TaskInfo temp = task->prev;

	if (gh_sch.root_task == task) {
		gh_sch.root_task = NULL;
		gh_sch.running_task = NULL;
		gh_sch.child_task = 0;
	} else {
		temp->next = task->next;

		if (task == gh_sch.running_task) {
			if (temp->next != NULL) {
				(task->next)->prev = temp;
				gh_sch.running_task = temp->next;
			} else {
				gh_sch.running_task = temp;
			}
		}

		gh_sch.child_task--;
	}

	free(task);
}
