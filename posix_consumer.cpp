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
#include <chrono>

// Global Queue Definitions for Queue Descriptor

#define QUEUE_NAME "/tps-queue"         // Name of our Queue
#define QUEUE_PERMISSIONS ((int)(0644)) // Permissions the Queue will have
#define QUEUE_NUM_MSG 20                // Max Number of Messages Allowed in the Queue
#define QUEUE_MSG_SIZE 1024             // Max Message Size
#define QUEUE_ATTRIBUTES_INITIALISER ((struct mq_attr){0, QUEUE_NUM_MSG, QUEUE_MSG_SIZE, 0, {0}})

// In case CONSUMER is faster than the PUBLISHER

#define QUEUE_POLL_CONSUMER ((struct timespec){2, 500000000})
#define QUEUE_POLL_PUBLISHER ((struct timespec){5, 0})

#define QUEUE_MAX_PRIORITY ((int)(9))

// Main Driver Function

int main(int argc, char *argv[])
{

    struct mq_attr attr;          // Structure to Store Queue Attributes
    mqd_t mqdes;                  // Queue Descriptor
    char buf[QUEUE_MSG_SIZE + 1]; // Buffer Initilisation
    unsigned int priority;        // Queue Message Priority
    std::string input_message;    // Message Input from User/CLI

    // Initialise Queue Attrbiute Structure

    attr.mq_maxmsg = QUEUE_NUM_MSG;
    attr.mq_msgsize = QUEUE_MSG_SIZE;
    attr.mq_flags = 0;
    priority = 0;

    // Create and Open a Queue with set Attributes

    mqdes = mq_open(QUEUE_NAME, O_RDONLY, QUEUE_PERMISSIONS, &attr);

    // If Unable to Create Queue Print Error and Quit
    if (mqdes < 0)
    {

        perror("mq_open()");
        exit(1);
    }

    // Run a Loop Until User Exits
    for (;;)
    {

        // Check IF Queue Has Messages
        if (attr.mq_curmsgs != 0)
        {

            // Allocate Memery for Message
            memset(buf, '\0', QUEUE_MSG_SIZE + 1);

            ssize_t bytes_read = mq_receive(mqdes, buf, QUEUE_MSG_SIZE, &priority); // Store Message Size

            // Check if Any Bytes Received
            if (bytes_read > 0)
            {

                buf[bytes_read] = '\0';
                printf("Bytes Recieved = %zd, Message = %s \n", bytes_read, buf);
            }

            if (bytes_read == 3 && !strcmp(buf, "exit"))
            {

                break;
            }
        }
    }

    mq_close(mqdes); // Close Queue

    return (0);
}