#include "app_uart.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "bsp.h"
#include "nrf_uart.h"


#include "usart_api.h"

#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 256                         /**< UART RX buffer size. */

#define UART_HWFC APP_UART_FLOW_CONTROL_DISABLED

static int usart_init_impl();
// static char usart_getchar();

usart_api_t usart_expose_api = {
	.usart_init = usart_init_impl,
	.usart_putchar = app_uart_put,
	.usart_getchar = app_uart_get
};

// Required for nrf52 uart setup
static void uart_error_handle(app_uart_evt_t * p_event) {}

static int usart_init_impl() {
	uint32_t err_code;

	const app_uart_comm_params_t comm_params =
	{
		RX_PIN_NUMBER,
		TX_PIN_NUMBER,
		RTS_PIN_NUMBER,
		CTS_PIN_NUMBER,
		UART_HWFC,
		false,
		NRF_UART_BAUDRATE_115200
	};

	APP_UART_FIFO_INIT(&comm_params,
						UART_RX_BUF_SIZE,
						UART_TX_BUF_SIZE,
						uart_error_handle,
						APP_IRQ_PRIORITY_LOWEST,
						err_code);
	APP_ERROR_CHECK(err_code);

	return 0;
}