#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#include "client.h"
#include "CLI.h"

#define MAX_SIZE 100
#define MAX_COMMANDS 4

Commands_s commands[] = {
    { "connect", "Command is for connecting. connect <ip_addr> <port>", &connecting },
    { "disconnect", "Command is for disconnecting", &disconnecting },
    { "get_time", "Command gives current time", &get_time },
    { "prompt", "Choosing a new prompt", &set_prompt }
};

int connected = 0;
int listening = -2;

void connecting(char **pp_in, int length) {
    if (length == 3) {
        if (connected == 1) {
            printf(" Is already connected.\n");
            return;
        }
        int port = atoi(pp_in[2]);
        int ip_address = atoi(pp_in[1]);

        listening = socket(AF_INET, SOCK_STREAM, 0);
        if (listening == -1) {
            printf(" Error in connection.\n");
            return;
        }
 
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = INADDR_ANY;
        server_address.sin_port = htons(port);

        int con = connect(listening, (struct sockaddr*)&server_address, sizeof(server_address));
        if (con == -1) {
            printf(" Error in connection.\n");
            return;
        }
 
        char *p_text = (char*)malloc(MAX_SIZE);
        char *p_response = (char*)malloc(MAX_SIZE);
        strcat(p_text, pp_in[1]);
        strcat(p_text, " ");
        strcat(p_text, pp_in[2]);
        send(listening, p_text, strlen(p_text), 0);
        
        printf(" Connecting to port: %s, ip_address: %s\n", pp_in[2], pp_in[1]); 
        connected = 1;
        
        free(p_text);
        free(p_response);
    } else {
        printf(" Error: No port or ip_address was given.\n");
    }
}

void disconnecting(char **pp_in, int length) {
    if (length == 1) {
        if (connected == 1) {
            send(listening, pp_in[0], strlen(pp_in[0]), 0);
            close(listening);
            connected = 0;
            printf(" Disconnected from the server.\n");
        } else {
            printf(" There is no connection to disconnect.\n");
        }
    } else {
        printf(" Error: Wrong use of command. No argument needed.\n");
    }
}

void get_time(char **pp_in, int length) {
    char *p_response = (char*)malloc(MAX_SIZE);

    if (length == 1) {
        if (connected == 1) {
            send(listening, pp_in[0], strlen(pp_in[0]), 0);
            recv(listening, p_response, MAX_SIZE, 0);
            printf(" Current time is %s\n", p_response);
        } else {
            printf(" There is no connection to server.\n");
        }
    } else {
        printf(" Error: Wrong use of command. No argument needed.\n");
    }
    free(p_response);
}

char prompt[MAX_SIZE];

void set_prompt(char **pp_in, int length) {
    if (length == 2) {
        strcpy(prompt, pp_in[1]);
    } else {
        printf(" Error: Wrong use of command. No prompt was given.\n");
    }
}

int main() {
    char *p_input = (char*)malloc(MAX_SIZE);
    strcpy(prompt, ">");

    while (1) {
        print_prompt(prompt);
        fgets(p_input, MAX_SIZE, stdin);

        if (strlen(p_input) > 1) {
            int commands_count = sizeof(commands) / sizeof(commands[0]);
            run_cli(commands, commands_count, p_input);
        }    
    }
    free(p_input);
}

