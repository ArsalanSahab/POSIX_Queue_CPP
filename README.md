# Steps to Compile and Run

## Pre-Requesits :

    * Linux,Debian or Unix Distribution.
    * Must have g++ installed : `sudo apt install build-essential`.

## How to Compile

    * Naviagte to the directory containing the Makefile, posix_consumer.cpp file and posix_publisher.cpp file
    * In the Terminal type : ` make `

## How to Use

    * Open Two Terminal Tabs side by side.
    * In the First Tab type : ` ./posix_publisher `.
    * In the Second Tab type : ` ./posix_consumer ` OR `./posix_multithread_consumer`.
    * In the Publisher tab you can enter messages, in the consumer tab you will see the output.
    * To exit type `exit` in the Publisher tab.
