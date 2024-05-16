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
        int sendfd, recvfd;
        int cnt = 5;
        int score = 100;
        char buf[BUF_SIZE];
	sem_t* sem;
        sem_t* sem2;
	sem_t* sem3;

	sem_unlink(SEM_NAME);
	sem_unlink(SEM_NAME2);
	sem_unlink(SEM_NAME3);

	if((sem = sem_open(SEM_NAME, O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("sem_open() error");
		return -1;
	}
	if((sem2 = sem_open(SEM_NAME2, O_CREAT, 0644, 0)) == SEM_FAILED) {
		perror("sem_open() error");
		return -1;
	}
	if((sem3 = sem_open(SEM_NAME3, O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("sem_open() error");
		return -1;
	}

	mkfifo(FIFO_TEMP, 0666); // fifo file

        sendfd = open(FIFO_TEMP, O_WRONLY);

        if (sendfd == -1) {
                perror("fifo open error!");
                return -1;
        }

        while (1) {
		sem_wait(sem);
                memset(buf, 0x00, BUF_SIZE);
                printf("Your turn!\n");
                fgets(buf, BUF_SIZE, stdin); // give an answer to client
                write(sendfd, buf, strlen(buf));
		sem_post(sem);

                if (strcmp(buf, "ping\n")) { // check answer with ping
                        printf("wrong! -20\n");
                        score -= 20;
                }

                if (cnt == 5) { // if first incounter, open fifo_client.
                                // this structure will be described in report
                        recvfd = open(FIFO_TEMP, O_RDONLY);
                        if (recvfd == -1) {
                                perror("fifo open error!");
                                return -1;
                        }
                }

		sem_wait(sem2);
		
		sem_wait(sem3);
                memset(buf, 0x00, BUF_SIZE);
                read(recvfd, buf, BUF_SIZE); // get client's answer
		sem_post(sem3);

                if (--cnt == 0) { // if break before client end, client's fifo conenction will be lost
                                  // for that, break check is inserted after get client's answer == client's end
                        break;
                }
                printf("[opponent] %s", buf);
        }

        printf("Done! Your score : %d\n", score);
        close(sendfd);
        close(recvfd);

	sem_close(sem);
	sem_close(sem2);
	sem_close(sem3);
}
