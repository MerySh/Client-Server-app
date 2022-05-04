/*
 * brief: runs CLI for current application
 *
 * commands: array of all application commands
 * commands_count: number of commands
 * input: inputed text from user
 *
 * returns: nothing
 */
void run_cli(Commands_s *p_commands, int commands_count, char *p_input);

/*
 * brief: parses inputed string from user
 *
 * input: inputed text from user
 * parsed_input: parsed text from CLI
 * 
 * returns: nothing
 */
void parse(char *p_input, char **pp_parsed_input, int *p_count_parsed_input);

/*
 * brief: prints application's prompt
 *
 * prompt: application's prompt
 * 
 * returns: nothing
 */
void print_prompt(char *p_prompt);

/*
 * brief: prints application's all command's name and info
 *
 * commands: array of all application commands
 * commands_count: number of commands
 * 
 * returns: nothing
 */
void print_all_commands(Commands_s *p_commands, int commands_count);

/*
 * brief: checks if it is a command or not
 *
 * commands: array of all application commands
 * commands_count: number of commands
 * parsed_input: parsed text from CLI
 * idx: current index
 * command: current command
 * 
 * returns: nothing
 */
int check_iscommand(Commands_s *p_commands, int commands_count, char **pp_parsed_input, int *p_idx, char *p_command);
