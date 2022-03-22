#include "lib_test.h"
#include "secure_channel.h"
#include <stdint.h>

typedef struct func_api_t {
	int (*func_test1)(int op1, int op2);
	int (*func_test2)(int op1);
} func_api_t;

static void copy_data_bss() {
	uint32_t __etext = 0x5100;
	uint32_t __data_start__ = 0x20003000;
	uint32_t __data_end__ = 0x2000301c;
	for (uint32_t *dst = __data_start__, *src = __etext; dst < __data_end__; ) {
		*(dst++) = *(src++);
	}
}

void test_func_lib() {
	copy_data_bss();
	const func_api_t *api = (const func_api_t *) 0x5000;
	debug_log("call test1 func: test1(%d + %d) = %d\n", 3, 5, api->func_test1(3, 5));
	for (int t = 0; t < 10; t++) {
		debug_log("call test1 func: test2(%d) = %d\n", t, api->func_test2(t));
	}
	debug_log("\n-------------flush-----------\n");
}

typedef struct usart_api_t {
	int (*usart_init)();
	uint32_t (*usart_putchar)(uint8_t ch);
	uint32_t (*usart_getchar)(uint8_t *);
} usart_api_t;

// usart_api_t __start_usart_expose_api;
// usart_api_t __end_usart_expose_api;

void test_bin() {
	register char * stack_ptr asm("sp");
	// printf("printf test\n");

	usart_api_t *api_exposed = (usart_api_t *) 0x00004880;
	debug_log("usart_init: 0x%08x\n", api_exposed->usart_init);
	debug_log("usart_putchar: 0x%08x\n", api_exposed->usart_putchar);
	debug_log("usart_getchar: 0x%08x\n", api_exposed->usart_getchar);
	for (int i = 0; i < 10; i++) {
		// api_exposed->usart_putchar("t");
	}
	debug_log("after function call: 0x%08x\n", stack_ptr);
}
