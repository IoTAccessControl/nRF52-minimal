#include "shell.h"


#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

/*
1. 
2. using section to load cmd
*/

static int cli_cmd_hello(int argc, char *argv[]) {
    shell_put_line("hello\n");
    return 0;
}

// static const ShellCommand s_shell_commands[] = {
//     {"help", shell_help_handler, "List all commands"},
//     {"hello", cli_cmd_hello, "Say Hello"}
// };

// const ShellCommand * const g_shell_commands = s_shell_commands;
// const int g_num_shell_commands = ARRAY_SIZE(s_shell_commands);

SHELL_CMD(help, shell_help_handler, "List all commands");
SHELL_CMD(hello, cli_cmd_hello, "Say hello");