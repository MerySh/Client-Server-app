#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <syslog.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#include "client.h"
#include "CLI.h"

#define MAX_CLIENTS 5
#define MAX_SIZE 100

void parse_arguments(int argc, char **argv, char *p_ip_address, char *p_port) 
{
    /*
    int opt;
    while ((opt = getopt(argc, argv, "i:p:")) != -1) {
        switch(opt) {
        case 'i' :
            strcpy(p_ip_address, optarg);
            break;
        case 'p' :
            strcpy(p_port, optarg);
            break;
        default :
            printf("No arguments\n");
            break;
        }
    }
    */

    strcpy(p_ip_address, argv[2]);
    strcpy(p_port, argv[4]);
}

int main(int argc, char **argv) 
{
    if (argc != 5) {
        printf("Error: no port or IP address was given.\n");
        exit(1);
    }

    // for daemon
    pid_t pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);
    chdir("/");

    for (int x = sysconf(_SC_OPEN_MAX); x >= 0; --x) {
        close(x);
    }

    openlog ("daemon", LOG_PID, LOG_DAEMON);
    ///////////////////////////////////////////////

    char *p_ip_address = (char*)malloc(MAX_SIZE);
    char *p_port = (char*)malloc(MAX_SIZE);
    parse_arguments(argc, argv, p_ip_address, p_port);

    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1) {
        printf("Error in connection.\n");
        exit(1);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(atoi(p_port));

    int optional = 1;
    setsockopt(listening, SOL_SOCKET, SO_REUSEADDR, &optional, sizeof(optional));

    int binding = bind(listening, (struct sockaddr*)&server_address, sizeof(server_address));
    if (binding == -1) {
        printf("Error in binding.\n");
        exit(1);
    }

    if (listen(listening, MAX_CLIENTS) == 0) {
        printf("Listening for the client...\n");
    } else {
        printf("Error in listening.\n");
        exit(1);
    }

    int client_socket;
    struct sockaddr_in new_address;
    char *p_buffer1 = (char*)malloc(MAX_SIZE);
    char *p_buffer2 = (char*)malloc(MAX_SIZE);
    
    while (1) {
        client_socket = accept(listening, NULL, NULL);  
        if (client_socket == -1) {
            exit(1);
        }
 
        char port[MAX_SIZE];
        char ip_addr[MAX_SIZE];
        recv(client_socket, p_buffer1, 2048, 0);
        for (int i = 0; i < strlen(p_buffer1); ++i) {
            if (p_buffer1[i] != ' ') {
                ip_addr[i] = p_buffer1[i];   
            } else {
                int k = 0;
                for (int j = i+1; j < strlen(p_buffer1); ++j) {
                    port[k++] = p_buffer1[j];
                }
                break;
            }
        }
        if (!strncmp(argv[1], port, 4)) {            
            printf("Connection accepted from %s:%s\n", ip_addr, port); 
        }

        while (1) {
            recv(client_socket, p_buffer2, 2048, 0);
            if (!strncmp(p_buffer2, "disconnect", 10)) {
                printf("Disconnected from %s:%s\n", ip_addr, port);
                break;
            } else if (!strncmp(p_buffer2, "get_time", 8)) {
                time_t current_time = time(NULL);
                char buff[MAX_SIZE];
                strftime(buff, MAX_SIZE, "%Y-%m-%d %H:%M:%S", localtime(&current_time));
                send(client_socket, &buff, MAX_SIZE, 0);
            }
        }
    }
    free(p_ip_address);
    free(p_port);
    free(p_buffer1);
    free(p_buffer2);
}
