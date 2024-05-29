#include <stdbool.h>
#include <stdio.h>

#include <unistd.h>

#include "scheduler.h"

void test_func_one() {
	printf("Start test_func_one()\n");

	for (int i = 0; i <= 15; i++) {
		printf("TASK 1 : %5d\n", i);
		sleep(1);
	}

	printf("End of the test_func_one()\n");
}

void test_func_two() {
	printf("Start test_func_two()\n");

	for (int i = 500; i <= 700; i += 10) {
		printf("\t\t\tTASK 2 : %5d\n", i);
		sleep(1);
	}

	printf("End of the test_func_two()\n");
}


void test_func_three() {
	printf("Start test_func_three()\n");

	for (int i = 1000; i <= 1005; i++) {
		printf("\t\t\t\t\t\tTASK 3 : %5d\n", i);
		sleep(1);
	}

	printf("End of the test_func_three()\n");
}

int main() {
	thread_init();

	thread_create(test_func_one, NULL);
	thread_create(test_func_two, NULL);
	thread_create(test_func_three, NULL);

	thread_wait();

	return 0;
}
