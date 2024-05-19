#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>

#define BUF_SIZE 100
#define FIFO_TEMP "./fifo_temp"
#define SEM_NAME "mysema"
#define SEM_NAME2 "mysema2"
#define SEM_NAME3 "mysema3"

int main() {
        int recvfd, sendfd;
        int cnt = 5;
        int score = 100;
        char buf[BUF_SIZE];
	sem_t* sem;
	sem_t* sem3;

	if ((sem = sem_open(SEM_NAME, O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("sem_open() error");
		return -1;
	}
	if ((sem3 = sem_open(SEM_NAME3, O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("sem_open() error");
		return -1;
	}

        recvfd = open(FIFO_TEMP, O_RDONLY);
        sendfd = open(FIFO_TEMP, O_WRONLY); // open fifo file

        if (recvfd == -1 || sendfd == -1) { // file open error
                perror("fifo open error!");
                return -1;
        }

        while (1) {
		sem_wait(sem3);
                memset(buf, 0x00, BUF_SIZE);
                read(recvfd, buf, BUF_SIZE); // read server's answer
                printf("[opponent] %s", buf);
		sem_post(sem3);

		sem_wait(sem);
                memset(buf, 0x00, BUF_SIZE);
                printf("Your turn!\n");
                fgets(buf, BUF_SIZE, stdin); // get client's answer
                write(sendfd, buf, strlen(buf)); // write client's answer

                if (strcmp(buf, "pong\n")) { // compare answer with "pong"
                        printf("wrong! -20\n");
                        score -= 20; // -20 point
                }
                if (--cnt == 0) { // if 5 times passed, break
                        break;
                }
		sem_post(sem);
        }

        printf("Done! Your score : %d\n", score); // show result
        close(sendfd);
        close(recvfd);

	sem_close(sem);
	sem_close(sem3);
}
