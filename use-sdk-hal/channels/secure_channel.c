#include "secure_channel.h"

#include <stdint.h>
#include "usart_api.h"

#define NRF_SUCCESS 0

int init_secure_channel() {
	return usart_expose_api.usart_init();
}

int secure_putc(char c) {
	// app_uart_put(c);
	usart_expose_api.usart_putchar(c);
	return 1;
}

char secure_getc(void) {
	uint8_t cr = 0;
	while (app_uart_get(&cr) != NRF_SUCCESS);
	return (char)cr;
}