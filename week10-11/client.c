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
        int recvfd, sendfd;
        int cnt = 5;
        int score = 100;
        char buf[BUF_SIZE];
	sem_t* server_turn;
	sem_t* client_turn;

	if ((server_turn = sem_open(SEM_NAME1, O_CREAT, 0644, 0)) == SEM_FAILED) {
		perror("sem_open() error");
		return -1;
	}
	if ((client_turn = sem_open(SEM_NAME2, O_CREAT, 0644, 0)) == SEM_FAILED) {
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
		sem_wait(client_turn);

                memset(buf, 0x00, BUF_SIZE);
                read(recvfd, buf, BUF_SIZE); // read server's answer
                printf("[opponent] %s", buf);

                memset(buf, 0x00, BUF_SIZE);
                printf("Your turn!\n");
                fgets(buf, BUF_SIZE, stdin); // get client's answer
                write(sendfd, buf, strlen(buf)); // write client's answer

                if (strcmp(buf, "pong\n")) { // compare answer with "pong"
                        printf("wrong! -20\n");
                        score -= 20; // -20 point
                }
		sem_post(server_turn);
                if (--cnt == 0) { // if 5 times passed, break
                        break;
                }
        }

        printf("Done! Your score : %d\n", score); // show result
						  
        close(sendfd);
        close(recvfd);

	sem_close(server_turn);
	sem_close(client_turn);
}
