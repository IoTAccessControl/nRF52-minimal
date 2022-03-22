#include "shell.h"
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#define SHELL_RX_BUFFER_SIZE 256
#define SHELL_MAX_ARGS 16
#define SHELL_PROMPT "$ "

struct ShellCommand __shell_cmd_sec_start__;
struct ShellCommand __shell_cmd_sec_end__;

static struct ShellContext {
    int (*send_char)(char c);
    int rx_size;
    char rx_buffer[SHELL_RX_BUFFER_SIZE]; // one line
} shell_ctx;

static void prv_send_char(char c) {
    if (shell_ctx.send_char != NULL) {
        shell_ctx.send_char(c);
    }
}

static void prv_echo(char c) {
    if ('\n' == c) {
        prv_send_char('\r');
        prv_send_char('\n');
    } else if ('\b' == c) {
        prv_send_char('\b');
        prv_send_char(' ');
        prv_send_char('\b');
    } else {
        prv_send_char(c);
    }
}

static void prv_echo_str(const char *str) {
    for (const char *c = str; *c != '\0'; ++c) {
        prv_echo(*c);
    }
}

static void reset_shell_buffer() {
    memset(shell_ctx.rx_buffer, 0, sizeof (shell_ctx.rx_buffer));
    shell_ctx.rx_size = 0;
}

static inline bool prv_buffer_is_full() {
    return shell_ctx.rx_size >= SHELL_RX_BUFFER_SIZE;
}

static const ShellCommand * prv_find_command(char *name) {
    // for (int i = 0; i < g_num_shell_commands; i++) {
    //     const ShellCommand * command = &g_shell_commands[i];
    //     if (strcmp(command->command, name) == 0) {
    //         return command;
    //     }
    // }
    for (ShellCommand *cmd = &__shell_cmd_sec_start__; cmd < &__shell_cmd_sec_end__; cmd++) {
        if (strcmp(cmd->command, name) == 0) {
            return cmd;
        }
    }
    return NULL;
}

static void prv_process() {
    char last = shell_ctx.rx_buffer[shell_ctx.rx_size - 1];
    if (last != '\n' && !prv_buffer_is_full()) {
        return;
    }
    // prv_echo_str("not return\n");
    char *argv[SHELL_MAX_ARGS] = {0};
    int argc = 0;
    char *next_arg = NULL;
    for (int i = 0; i < shell_ctx.rx_size && argc < SHELL_MAX_ARGS; i++) {
        char *const c = &shell_ctx.rx_buffer[i];
        if (*c == ' ' || *c == '\n' || i == shell_ctx.rx_size - 1) {
            *c = '\0';
            if (next_arg) {
                argv[argc++] = next_arg; 
                next_arg = NULL;
            }
        } else if (next_arg == NULL) {
            next_arg = c;
        }
    }

    if (shell_ctx.rx_size == SHELL_RX_BUFFER_SIZE) {
        prv_echo('\n');
    }

    if (argc >= 1) {
        const ShellCommand *command = prv_find_command(argv[0]);
        if (!command) {
            prv_echo_str("Unkown command: ");
            prv_echo_str(argv[0]);
            prv_echo_str("\n");
            prv_echo_str("Type 'help' to list all commands\n");
        } else {
            command->handler(argc, argv);
        }
    }

    reset_shell_buffer();
    prv_echo_str(SHELL_PROMPT);
}

void shell_boot(const ShellImpl *impl) {
    shell_ctx.send_char = impl->send_char;
    reset_shell_buffer();
    prv_echo_str("\n" SHELL_PROMPT);
}

void shell_put_line(const char *str) {
    prv_echo_str(str);
    prv_echo_str("\n");
}


void shell_receive_char(char c) {
    if (c == '\r' || prv_buffer_is_full() || shell_ctx.send_char == NULL) {
        return;
    }
    prv_echo(c);

    if (c == '\b') {
        shell_ctx.rx_buffer[--shell_ctx.rx_size] = '\0';
        return;
    }
    shell_ctx.rx_buffer[shell_ctx.rx_size++] = c;
    prv_process();
}

int shell_help_handler(int argc, char *argv[]){
     // for (int i = 0; i < g_num_shell_commands; i++) {
     //    const ShellCommand * command = &g_shell_commands[i];
     //    prv_echo_str(command->command);
     //    prv_echo_str(": ");
     //    prv_echo_str(command->help);
     //    prv_echo('\n');
     // }
     return 0;
}
