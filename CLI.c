#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "client.h"
#include "CLI.h"

#define MAX_SIZE 100

void parse(char *p_input, char **pp_parsed_input, int *p_count_parsed_input) 
{
    int j = 0;
    int c = *p_count_parsed_input;
    for (int i = 0; i < strlen(p_input); ++i) {
        if (p_input[i] == ' ' || p_input[i] == '\0' || p_input[i] == '\n') {
            pp_parsed_input[c][j] = '\0';
            ++c;
            j = 0;
        } else {
            pp_parsed_input[c][j] = p_input[i];
            ++j;
        }
    }
    *p_count_parsed_input = c;
}

void print_prompt(char *p_prompt) {
    printf("%s ", p_prompt);
}

void print_all_commands(Commands_s *p_commands, int commands_count) {
    int max_size = 0; 
    for (int i = 0; i < commands_count; ++i) {
        if (strlen(p_commands[i].p_name) > max_size) {
            max_size = strlen(p_commands[i].p_name);
        }
    }
    for (int i = 0; i < commands_count; ++i) {
        int space = max_size - strlen(p_commands[i].p_name) + 1;
        printf(" %s %*c %s\n", p_commands[i].p_name, space, '|', p_commands[i].p_info); 
    }
}

int check_iscommand(Commands_s *p_commands, int commands_count, char **pp_parsed_input, int *p_idx, char *p_command) {
    int flag = 0;
    for (int i = 0; i < commands_count; ++i) {
        if (!strcmp(pp_parsed_input[0], p_commands[i].p_name)) {
            strcpy(p_command, pp_parsed_input[0]);
            *p_idx = i;
            flag = 1;
            break;
         }
    }
    return flag;
}

/*
 * Brief: runs CLI for current application, parses inputed text and checks
 *        if it is a command then calls current command's function handler
 *        if it is a <help> then prints all commands' information 
 *        if it is a command + <--help> then prints current command's information
 */
void run_cli(Commands_s *p_commands, int commands_count, char *p_input) {
    char **pp_parsed_input = (char**)malloc(MAX_SIZE * sizeof(char*));
    for (int i = 0; i < MAX_SIZE; ++i) {
        pp_parsed_input[i] = (char*)malloc(MAX_SIZE * sizeof(char));    
    }

    int count_parsed_input = 0;
    parse(p_input, pp_parsed_input, &count_parsed_input);

    if (count_parsed_input >= 1) {
        int idx = 0;
        char command[MAX_SIZE];
        int is_command = check_iscommand(p_commands, commands_count, pp_parsed_input, &idx, command);
        
        if (!is_command) { 
            if (!strcmp(pp_parsed_input[0], "help") && count_parsed_input == 1) {
                print_all_commands(p_commands, commands_count);
            } else if (!strcmp(pp_parsed_input[0], "help") && count_parsed_input != 1) {
                printf(" Error: Wrong use of command.\n");
            } else {
                printf(" Error: command not found.\n");
            }
        } else {
            if (!strcmp(pp_parsed_input[1], "--help") && count_parsed_input == 2) { 
                printf(" %s\n", p_commands[idx].p_info);
            } else if (!strcmp(pp_parsed_input[1], "--help") && count_parsed_input > 2) {
                printf(" Error: command not found.\n");
            } else {
                p_commands[idx].Command(pp_parsed_input, count_parsed_input); 
            }
        }
    }
}


