#ifndef USART_API_H_
#define USART_API_H_
#include <stdint.h>

typedef struct usart_api_t {
	int (*usart_init)();
	uint32_t (*usart_putchar)(uint8_t ch);
	uint32_t (*usart_getchar)(uint8_t *);
	uint32_t (*usart_flush)(void);
} usart_api_t;

// extern usart_api_t usart_expose_api;

#endif
