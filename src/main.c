#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../headers/server.h"

#define DEFAULT_PORT 8080

void show_help(char* cmd) {
    printf("usage %s [path] [options]\n\n\
options:\n\
 -p --port\tPort to use [8080]\n\
 -h --help\tPrint this list and exit.\n", 
        cmd);
}

int main(int argc, char** argv)
{
    char* cwd = NULL;
    int port = DEFAULT_PORT;
   
    char* cmd = *argv++;
    argc--;

    while (argc-- > 0) {
        if(cwd == NULL && **argv != '-')
            cwd = *argv;

        if(!strcmp("-h", *argv) || !strcmp("--help", *argv)) {
            show_help(cmd);
            exit(0);
        }

        if((!strcmp("-p", *argv) || !strcmp("--port", *argv)) && argc > 0) {
            argv++;
            argc--;
            port = atoi(*argv);
        }

        argv++;
    }

    if(cwd != NULL) {
        if(chdir(cwd) < 0) {
            perror("Settiing path failed");
            exit(1);
        }
    }
    
    printf("Starting up server...\n");
    printf("Serving: %s\n", cwd ? cwd : "./");
    create_server(port);
}