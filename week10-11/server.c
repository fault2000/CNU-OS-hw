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
	sem_t* sem3;

	sem_unlink(SEM_NAME);
	sem_unlink(SEM_NAME3);

	if((sem = sem_open(SEM_NAME, O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("sem_open() error");
		return -1;
	}
	if((sem3 = sem_open(SEM_NAME3, O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("sem_open() error");
		return -1;
	}

	mkfifo(FIFO_TEMP, 0666); // fifo file

        sendfd = open(FIFO_TEMP, O_WRONLY);
        recvfd = open(FIFO_TEMP, O_RDONLY);

        if (sendfd == -1 || recvfd == -1) {
                perror("fifo open error!");
                return -1;
        }

        while (1) {
		sem_wait(sem);
                memset(buf, 0x00, BUF_SIZE);
                printf("Your turn!\n");
                fgets(buf, BUF_SIZE, stdin); // give an answer to client
                write(sendfd, buf, strlen(buf));

                if (strcmp(buf, "ping\n")) { // check answer with ping
                        printf("wrong! -20\n");
                        score -= 20;
                }

		sem_post(sem);

		sem_wait(sem3);
                memset(buf, 0x00, BUF_SIZE);
                read(recvfd, buf, BUF_SIZE); // get client's answer

                if (--cnt == 0) { // if break before client end, client's fifo conenction will be lost
                                  // for that, break check is inserted after get client's answer == client's end
                        break;
                }

                printf("[opponent] %s", buf);
		sem_post(sem3);
        }

        printf("Done! Your score : %d\n", score);
        close(sendfd);
        close(recvfd);

	sem_close(sem);
	sem_close(sem3);
}
