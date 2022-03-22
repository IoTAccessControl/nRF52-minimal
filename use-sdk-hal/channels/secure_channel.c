#include "secure_channel.h"

#include <stdint.h>
#include <string.h>
#include <stdarg.h>
// #include "usart_api.h"

#define NRF_SUCCESS 0

typedef struct usart_api_t {
	int (*usart_init)();
	uint32_t (*usart_putchar)(uint8_t ch);
	uint32_t (*usart_getchar)(uint8_t *);
} usart_api_t;


#ifdef USE_SPLIT_BIN
const usart_api_t *__start_usart_expose_api = (const usart_api_t *) 0x00004880;

static const usart_api_t* get_usart_api() {
	// return &__start_usart_expose_api;
	return __start_usart_expose_api;
}

#else

usart_api_t __start_usart_expose_api;

static const usart_api_t* get_usart_api() {
	return &__start_usart_expose_api;
}

#endif // end USE_SPLIT_BIN

int init_secure_channel() {
	// (void) usart_expose_api;
	// return usart_expose_api.usart_init();
	return get_usart_api()->usart_init();
}

int secure_putc(char c) {
	// app_uart_put(c);
	get_usart_api()->usart_putchar(c);
	return 1;
}

char secure_getc(void) {
	uint8_t cr = 0;
	while (get_usart_api()->usart_getchar(&cr) != NRF_SUCCESS);
	return (char)cr;
}

static log_print_porting(const char *fmt, va_list *args) {
	char log_buf[128];
	memset(log_buf, 0, sizeof(log_buf));
	vsnprintf(log_buf, sizeof(log_buf) - 1, fmt, *args);
	for (const char * c = log_buf; *c != 0; c++) {
		if (*c == '\n') {
			secure_putc('\r');
			secure_putc('\n');
		} else {
			secure_putc(*c);
		}
	}
}

void debug_log(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	log_print_porting(fmt, &args);
	// vsprintf(log, fmt, args);
	va_end(args);
}