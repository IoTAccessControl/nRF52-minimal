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

static int cli_cmd_hello_handler(int argc, char *argv[]) {
    shell_put_line("hello\n");
    return 0;
}

#include "lib_test.h"
static int cli_cmd_test_lib(int argc, char *argv[]) {
    test_func_lib();
    return 0;
}

// static const ShellCommand s_shell_commands[] = {
//     {"help", shell_help_handler, "List all commands"},
//     {"hello", cli_cmd_hello, "Say Hello"}
// };

// const ShellCommand * const g_shell_commands = s_shell_commands;
// const int g_num_shell_commands = ARRAY_SIZE(s_shell_commands);

SHELL_CMD(help, cli_cmd_hello_handler, "List all commands");
SHELL_CMD(hello, cli_cmd_hello, "Say hello");
SHELL_CMD(lib_test, cli_cmd_test_lib, "Run lib test");

struct ShellCommand shell_cmd_snk __attribute__((section("shell_cmd_sec"))) = {"snk", cli_cmd_hello_handler, "xxxx"};