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
#define SEM_NAME1 "mysema1"
#define SEM_NAME2 "mysema2"

int main() {
        int sendfd, recvfd;
        int cnt = 5;
        int score = 100;
        char buf[BUF_SIZE];
	sem_t* server_turn;
	sem_t* client_turn;

	sem_unlink(SEM_NAME1);
	sem_unlink(SEM_NAME2);

	if((server_turn = sem_open(SEM_NAME1, O_CREAT, 0644, 0)) == SEM_FAILED) {
		perror("sem_open() error");
		return -1;
	}
	if((client_turn = sem_open(SEM_NAME2, O_CREAT, 0644, 0)) == SEM_FAILED) {
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
                memset(buf, 0x00, BUF_SIZE);
                printf("Your turn!\n");
                fgets(buf, BUF_SIZE, stdin); // give an answer to client
                write(sendfd, buf, strlen(buf));

                if (strcmp(buf, "ping\n")) { // check answer with ping
                        printf("wrong! -20\n");
                        score -= 20;
                }

		sem_post(client_turn); // give a turn to client

		sem_wait(server_turn); // when client give server turn, start again.
                memset(buf, 0x00, BUF_SIZE);
                read(recvfd, buf, BUF_SIZE); // get client's answer
                printf("[opponent] %s", buf);

                if (--cnt == 0) { // check end
                        break;
                }

        }

        printf("Done! Your score : %d\n", score);

        close(sendfd);
        close(recvfd);

	sem_close(server_turn);
	sem_close(client_turn);
}
