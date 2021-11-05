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

using namespace std;

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

    memset(buf, '\0', sizeof(buf)); // Allocate/Set Memorty for Buffer

    /*

    // Reading From File

    FILE *file;

    file = fopen("test.txt", "r");
    if (file == NULL)
        return 1;

    //fseek(file, 0L, SEEK_END);
    //numbytes = ftell(file);
    //fseek(file, 0L, SEEK_SET);

    //fread(buf, sizeof(char), numbytes, file);

    printf("The file called test.dat contains this text\n\n%s", buf);

    /// ----------------- /// */

    // Create and Open a Queue with set attritbutes

    mqdes = mq_open(QUEUE_NAME, O_CREAT | O_RDWR | O_NONBLOCK, QUEUE_PERMISSIONS, &attr);

    // Print Error Message if Unable to Create Queue and Exit
    if (mqdes == -1)
    {
        perror("mq_open()");
        exit(1);
    }

    // Run a Loop Until User Exits

    for (;;)
    {

        std::cin.getline(buf, sizeof(buf)); // Get User Input into the Buffer
        /*

       
        fseek(file, 0L, SEEK_END);
        numbytes = ftell(file);
        fseek(file, 0L, SEEK_SET);

        fread(buf, sizeof(char), numbytes, file); */
        if (!strcmp(buf, "exit"))
        {

            break;
        }

        // Print Error if Unable to Send Message to the Queue
        if (mq_send(mqdes, buf, strlen(buf), priority) == -1)
        {

            perror("mq_send()");
        }
    }

    // Close all open messages/connections

    if (mq_send(mqdes, buf, strlen(buf), priority) == -1)
    {

        perror("mq_send()");
    }

    // Close Queue
    //fclose(file);
    mq_close(mqdes);
    return 0;
}