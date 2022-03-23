#include "lib_test.h"
#include "secure_channel.h"
#include <stdint.h>

extern uint32_t test_lib_hal_flash_addr;
extern uint32_t test_lib_data_flash_addr;
extern uint32_t test_lib_data_ram_addr;

typedef struct func_api_t {
	int (*func_test1)(int op1, int op2);
	int (*func_test2)(int op1);
} func_api_t;

static void copy_data_bss() {
	uint32_t __etext = &test_lib_data_flash_addr;
	uint32_t __data_start__ = &test_lib_data_ram_addr;
	uint32_t __data_end__ = (uint32_t) (&test_lib_data_ram_addr) + 20;
	for (uint32_t *dst = __data_start__, *src = __etext; dst < __data_end__; ) {
		*(dst++) = *(src++);
	}
}

static void dump_sp() {
	register char * stack_ptr asm("sp");
	debug_log("sp: 0x%08x\n", stack_ptr);
}

// uart 溢出
void test_func_lib() {
	dump_sp();
	copy_data_bss();
	const func_api_t *api = (const func_api_t *) (&test_lib_hal_flash_addr);
	debug_log("api_addr: 0x%08x 0x%08x 0x%08x\n", api, api->func_test1, api->func_test2);
	debug_log("call test1 func: test1(%d + %d) = %d\n", 3, 5, api->func_test1(3, 5));
	for (int t = 0; t < 7; t++) {
		debug_log("call test1 func: test2(%d) = %d\n", t, api->func_test2(t));
	}
	debug_log("\n-------------flush-----------\n");
	const uint32_t *data_start = (uint32_t *) (&test_lib_data_ram_addr);
	const uint32_t *data_end = (uint32_t *) ((uint32_t) (&test_lib_data_ram_addr) + 20);
	debug_log("addr: 0x%08x - 0x%08x\n", data_start, data_end);
	for (uint32_t * ptr = data_start; ptr < data_end; ptr++) {
		debug_log("addr: 0x%08x val:%d\n", ptr, *ptr);
	}
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
