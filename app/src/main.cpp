// Main program to build the application
// Has main(); does initialization and cleanup and perhaps some basic logic.

#include "program.h"

int main()
{
    Program program;

    program.program_init();

    printf("completeed!\n");

    //Eventually we can stop the application from socket,
    //and that will roll into cleanup naturally and end the application.
    sleep(500);
    //NEED TO ADD PROGRAM END ON FRONTEND

    program.program_cleanup();
}
