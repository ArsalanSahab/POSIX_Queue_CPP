// Required Header Files for MessagesQueue/POSIXQueue

#include <iostream>
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <chrono>


// Global Queue Definitions for Queue Descriptor

#define QUEUE_NAME "/tps-queue"         // Name of our Queue
#define QUEUE_PERMISSIONS ((int)(0644)) // Permissions the Queue will have
#define QUEUE_NUM_MSG 10                // Max Number of Messages Allowed in the Queue
#define QUEUE_MSG_SIZE 1024             // Max Message Size
#define QUEUE_ATTRIBUTES_INITIALISER ((struct mq_attr){0, QUEUE_NUM_MSG, QUEUE_MSG_SIZE, 0, {0}})

// In case CONSUMER is faster than the PUBLISHER

#define QUEUE_POLL_CONSUMER ((struct timespec){2, 500000000})
#define QUEUE_POLL_PUBLISHER ((struct timespec){5, 0})

#define QUEUE_MAX_PRIORITY ((int)(9))

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE);} while (0)


// Threaded Driver Function

static void posix_thread(union sigval sv) {


    struct mq_attr attr;
    ssize_t nr;
    void *buf;
    mqd_t mqdes = *((mqd_t *) sv.sival_ptr);

    std::cout << "POSIX Thread Initiated..." << std::endl;

    if(mq_getattr(mqdes, &attr) == -1) {

        handle_error("mq_getattr");

    }

    buf = malloc(attr.mq_msgsize);

    if(buf == NULL) {

        handle_error("malloc");
    }

    for (;;) {

        memset(buf, '\0', attr.mq_msgsize);

        time_t start, end;
        time(&start);

        nr = mq_receive(mqdes, (char*)buf, attr.mq_msgsize, NULL);

        time(&end);

        double time_taken = double(end-start);
        

        if(nr == -1) {

            handle_error("mq_receive");
        }

        printf("Read %zd Bytes from Queue, Message = %s, Time Taken = %f \n", nr, (char*)buf, time_taken);

        if(nr == 3 && !strcmp((char*)buf, "exit")) {

            break;
        }
    }

    free(buf);
    exit(EXIT_SUCCESS);

    

}


// Main Driver Function

int main(int argc, char *agrv[]) {

    mqd_t mqdes;
    struct sigevent sev;
    struct mq_attr attr;

    attr.mq_maxmsg = QUEUE_NUM_MSG;
    attr.mq_msgsize = QUEUE_MSG_SIZE;
    attr.mq_flags = 0;

    mqdes = mq_open(QUEUE_NAME, O_RDONLY, QUEUE_PERMISSIONS, &attr);

    if(mqdes == (mqd_t) -1) {

        handle_error("mq_open");
    }

    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = posix_thread;
    sev.sigev_notify_attributes = NULL;
    sev.sigev_value.sival_ptr = &mqdes;

    if(mq_notify(mqdes, &sev) == -1) {

        handle_error("mq_notify");
    }

    std::cout << "Waiting...." << std::endl;

    pause();
    exit(EXIT_SUCCESS);
}