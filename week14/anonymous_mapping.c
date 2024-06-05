#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main() {
	char *shared_memory;
	char message[] = "Hello, shared memory!";


