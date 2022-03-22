
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "shell.h"
#include "secure_channel.h"
#include "usart_api.h"

extern int _end;

void* _sbrk(int incr) {
	static unsigned char *heap = NULL;
	unsigned char *prev_heap;

	if (heap == NULL) {
		heap = (unsigned char *)&_end;
	}
	prev_heap = heap;

	heap += incr;

	return (void* ) prev_heap;
}

#include "lib_test.h"

int main(void) {
  // uint32_t err_code;
  // bsp_board_init(BSP_INIT_LEDS);

	char buf[128];
	char log[100];
	int len;

	init_secure_channel();

	ShellImpl impl = {.send_char = secure_putc};
	shell_boot(&impl);

	test_func_lib();

	char c;
	while (true) {
		c = secure_getc();
		shell_receive_char(c);
		// secure_putc(c);
	}
}