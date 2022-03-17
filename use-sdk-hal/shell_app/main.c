
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "shell.h"
#include "secure_channel.h"

 void console_putc(char ch) {
	secure_putc(ch);
}

char console_getc() {
	return secure_getc();
}

void console_puts(char *s) {
	while (*s != '\0') {
		if (*s == '\r') {
			console_putc('\n');
		} else {
			console_putc(*s);
		}
		s++;
	}
}

void console_put_line(char *s) {
	console_puts(s);
	console_puts("\n");
}

int console_gets(char *s, int len) {
	char *t = s;
	char c;

	*t = '\000';
	/* read until a <CR> is received */
	while ((c = secure_getc()) != '\n') {
	if ((c == '\010') || (c == '\127')) {
		if (t > s) {
		/* send ^H ^H to erase previous character */
		console_puts("\010 \010");
		t--;
		}
	} else {
		*t = c;
		console_putc(c);
		if ((t - s) < len) {
		t++;
		}
	}
	/* update end of string with NUL */
	*t = '\000';
	}
	return t - s;
}


int main(void) {
  // uint32_t err_code;
  // bsp_board_init(BSP_INIT_LEDS);

	char buf[128];
	char log[100];
	int len;

	init_secure_channel();

	ShellImpl impl = {.send_char = console_putc};
	shell_boot(&impl);

	char c;
	while (true) {
		c = console_getc();
		shell_receive_char(c);
		console_putc(c);
	}
}