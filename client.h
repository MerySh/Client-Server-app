/*
 * Commands struct which containts
 * command name, command info and function handler
 *
 */
typedef struct Commands {
    const char *p_name;
    const char *p_info;
    void (*Command)(char **pp_in, int length);
} Commands_s;

/*
 * Client's all commands' function handlers definitions
 */
void connecting();
void disconnecting();
void get_time();
void set_prompt();
