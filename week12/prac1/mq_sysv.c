#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define IPC_KEY_FILENAME      "/proc"
#define IPC_KEY_PROJ_ID       'a'
#define MSGBUF_STR_SIZE       64

typedef struct {
    long mtype;
    char string[MSGBUF_STR_SIZE];
} msgbuf;

static void print_usage(const char *progname) {
    printf("%s (send|recv) MTYPE\n", progname);
}

static int init_msgq() {
    int msgq;
    key_t key;

    key = ftok(IPC_KEY_FILENAME, IPC_KEY_PROJ_ID);

    if (key == -1) {
        perror("ftok()");
        return -1;
    }

    msgq = msgget(key, 0644 | IPC_CREAT);

    if (msgq == -1) {
        perror("msgget()");
        return -1;
    }

    return msgq;
}

static int do_send(long mtype) {
    int msgq;
    msgbuf nbuf;

    msgq = init_msgq();
    if (msgq == -1) {
        perror("init_msgq()");
        return -1;
    }

    memset(&nbuf, 0, sizeof(nbuf));
    nbuf.mtype = mtype;
    snprintf(nbuf.string, sizeof(nbuf.string), "hello world mtype %ld", mtype);

    if (msgsnd(msgq, &nbuf, sizeof(nbuf.string), 0) == -1) {
        perror("msgsnd()");
        return -1;
    }

    return 0;
}

static int do_recv(long mtype) {
    int msgq;
    msgbuf nbuf;

    msgq = init_msgq();
    if (msgq == -1) {
        perror("init_msgq()");
        return -1;
    }

    memset(&nbuf, 0, sizeof(nbuf));
    if (msgrcv(msgq, &nbuf, sizeof(nbuf.string), mtype, 0) == -1) {
        perror("msgrcv()");
        return -1;
    }

    printf("receive msg : mtype %ld, msg [%s]\n", nbuf.mtype, nbuf.string);

    return 0;
}

int main(int argc, char** argv) {
    long mtype;
    int ret;

    if (argc < 3) {
        print_usage(argv[0]);
        return -1;
    }

    mtype = strtol(argv[2], NULL, 10);

    if (!strcmp(argv[1], "send")) {
        if (mtype <= 0) {
            print_usage(argv[0]);
            return -1;
        }
        ret = do_send(mtype);
    } else if (!strcmp(argv[1], "recv")) {
        ret = do_recv(mtype);
    } else {
        print_usage(argv[0]);
        return -1;
    }

    return ret;
}

