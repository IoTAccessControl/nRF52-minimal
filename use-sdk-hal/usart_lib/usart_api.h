#ifndef USART_API_H_
#define USART_API_H_

typedef struct usart_api {
	int (*usart_init)();
	int (*usart_putchar)();
	char (*usart_getchar)();
} usart_api_t;

extern usart_api_t usart_expose_api;

#endif
