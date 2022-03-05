#pragma once

struct ShellCommand;


#define SHELL_CMD(name, handler, help) struct ShellCommand shell_cmd_##name \
    __attribute__((section("shell_cmd_sec"))) = {#name, handler, help};

// struct ShellCommand __shell_cmd_sec_start__;
// struct ShellCommand __shell_cmd_sec_end__;

typedef struct ShellCommand {
    const char *command;
    int (*handler)(int argc, char *argv[]);
    const char *help;
} ShellCommand;

typedef struct ShellImpl {
    int (*send_char)(char c);
} ShellImpl;

extern const ShellCommand *const g_shell_commands;
extern const int g_num_shell_commands;

void shell_boot(const ShellImpl *impl);

void shell_receive_char(char c);

int shell_help_handler(int argc, char *argv[]);

void shell_put_line(const char *str);