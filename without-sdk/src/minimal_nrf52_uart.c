//! @file
//! In the interest of brevity, an _extremely_ barebones port for the NRF52 UART Peripheral

#include "hal/uart.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cmsis_shim.h"


typedef struct {                                /*!< (@ 0x40002000) UART0 Structure                                            */
  __OM  uint32_t  TASKS_STARTRX;                /*!< (@ 0x00000000) Start UART receiver                                        */
  __OM  uint32_t  TASKS_STOPRX;                 /*!< (@ 0x00000004) Stop UART receiver                                         */
  __OM  uint32_t  TASKS_STARTTX;                /*!< (@ 0x00000008) Start UART transmitter                                     */
  __OM  uint32_t  TASKS_STOPTX;                 /*!< (@ 0x0000000C) Stop UART transmitter                                      */
  __IM  uint32_t  RESERVED[3];
  __OM  uint32_t  TASKS_SUSPEND;                /*!< (@ 0x0000001C) Suspend UART                                               */
  __IM  uint32_t  RESERVED1[56];
  __IOM uint32_t  EVENTS_CTS;                   /*!< (@ 0x00000100) CTS is activated (set low). Clear To Send.                 */
  __IOM uint32_t  EVENTS_NCTS;                  /*!< (@ 0x00000104) CTS is deactivated (set high). Not Clear To Send.          */
  __IOM uint32_t  EVENTS_RXDRDY;                /*!< (@ 0x00000108) Data received in RXD                                       */
  __IM  uint32_t  RESERVED2[4];
  __IOM uint32_t  EVENTS_TXDRDY;                /*!< (@ 0x0000011C) Data sent from TXD                                         */
  __IM  uint32_t  RESERVED3;
  __IOM uint32_t  EVENTS_ERROR;                 /*!< (@ 0x00000124) Error detected                                             */
  __IM  uint32_t  RESERVED4[7];
  __IOM uint32_t  EVENTS_RXTO;                  /*!< (@ 0x00000144) Receiver timeout                                           */
  __IM  uint32_t  RESERVED5[46];
  __IOM uint32_t  SHORTS;                       /*!< (@ 0x00000200) Shortcuts between local events and tasks                   */
  __IM  uint32_t  RESERVED6[64];
  __IOM uint32_t  INTENSET;                     /*!< (@ 0x00000304) Enable interrupt                                           */
  __IOM uint32_t  INTENCLR;                     /*!< (@ 0x00000308) Disable interrupt                                          */
  __IM  uint32_t  RESERVED7[93];
  __IOM uint32_t  ERRORSRC;                     /*!< (@ 0x00000480) Error source                                               */
  __IM  uint32_t  RESERVED8[31];
  __IOM uint32_t  ENABLE;                       /*!< (@ 0x00000500) Enable UART                                                */
  __IM  uint32_t  RESERVED9;
  __IOM struct {
    __IOM uint32_t  RTS;                          /*!< (@ 0x00000000) Pin select for RTS                                         */
    __IOM uint32_t  TXD;                          /*!< (@ 0x00000004) Pin select for TXD                                         */
    __IOM uint32_t  CTS;                          /*!< (@ 0x00000008) Pin select for CTS                                         */
    __IOM uint32_t  RXD;                          /*!< (@ 0x0000000C) Pin select for RXD                                         */
 } PSEL;  
  // __IOM UART_PSEL_Type PSEL;                    /*!< (@ 0x00000508) Unspecified                                                */
  __IM  uint32_t  RXD;                          /*!< (@ 0x00000518) RXD register                                               */
  __OM  uint32_t  TXD;                          /*!< (@ 0x0000051C) TXD register                                               */
  __IM  uint32_t  RESERVED10;
  __IOM uint32_t  BAUDRATE;                     /*!< (@ 0x00000524) Baud rate. Accuracy depends on the HFCLK source
                                                                    selected.                                                  */
  __IM  uint32_t  RESERVED11[17];
  __IOM uint32_t  CONFIG;                       /*!< (@ 0x0000056C) Configuration of parity and hardware flow control          */
} NRF_UART_Type;                                /*!< Size = 1392 (0x570)                                                       */


// What the NRF52 uart peripheral looks like
typedef struct {
  __OM  uint32_t  TASKS_STARTRX;
  __OM  uint32_t  TASKS_STOPRX;
  __OM  uint32_t  TASKS_STARTTX;
  __OM  uint32_t  TASKS_STOPTX;
  __IM  uint32_t  RESERVED[7];
  __OM  uint32_t  TASKS_FLUSHRX;
  __IM  uint32_t  RESERVED1[52];
  __IOM uint32_t  EVENTS_CTS;
  __IOM uint32_t  EVENTS_NCTS;
  __IOM uint32_t  EVENTS_RXDRDY;

  __IM  uint32_t  RESERVED2;
  __IOM uint32_t  EVENTS_ENDRX;
  __IM  uint32_t  RESERVED3[2];
  __IOM uint32_t  EVENTS_TXDRDY;
  __IOM uint32_t  EVENTS_ENDTX;
  __IOM uint32_t  EVENTS_ERROR;
  __IM  uint32_t  RESERVED4[7];
  __IOM uint32_t  EVENTS_RXTO;
  __IM  uint32_t  RESERVED5;
  __IOM uint32_t  EVENTS_RXSTARTED;
  __IOM uint32_t  EVENTS_TXSTARTED;
  __IM  uint32_t  RESERVED6;
  __IOM uint32_t  EVENTS_TXSTOPPED;
  __IM  uint32_t  RESERVED7[41];
  __IOM uint32_t  SHORTS;
  __IM  uint32_t  RESERVED8[63];
  __IOM uint32_t  INTEN;
  __IOM uint32_t  INTENSET;
  __IOM uint32_t  INTENCLR;
  __IM  uint32_t  RESERVED9[93];
  __IOM uint32_t  ERRORSRC;
  __IM  uint32_t  RESERVED10[31];
  __IOM uint32_t  ENABLE;
  __IM  uint32_t  RESERVED11;
  struct {
    __IOM uint32_t  RTS;
    __IOM uint32_t  TXD;
    __IOM uint32_t  CTS;
    __IOM uint32_t  RXD;
  } PSEL;
  __IM  uint32_t  RESERVED12[3];
  __IOM uint32_t  BAUDRATE;
  __IM  uint32_t  RESERVED13[3];
  struct {
    __IOM uint32_t  PTR;
    __IOM uint32_t  MAXCNT;
    __IM  uint32_t  AMOUNT;
  } RXD;
  __IM  uint32_t  RESERVED14;
  struct {
    __IOM uint32_t  PTR;
    __IOM uint32_t  MAXCNT;
    __IM  uint32_t  AMOUNT;
  } TXD;
  __IM  uint32_t  RESERVED15[7];
  __IOM uint32_t  CONFIG;
} sNrfUarteConfig;

#define NRF_UART0_BASE 0x40002000
static sNrfUarteConfig *const UARTE = ((sNrfUarteConfig *)0x40002000);

static NRF_UART_Type *const UART = ((NRF_UART_Type *) NRF_UART0_BASE);

static uint8_t s_rx_recv_buf[4];

static void prv_enable_nvic(int exti_id) {
  volatile uint32_t *nvic_ipr = (void *)(0xE000E400 + 4 * (exti_id / 4));
  *nvic_ipr = 0x00 << ((exti_id % 4) * 8);

  volatile uint32_t *nvic_iser = (void *)0xE000E100;
  *nvic_iser |= (1 << (exti_id % 32));
}

#define RX_PIN_NUMBER  8
#define TX_PIN_NUMBER  6
#define CTS_PIN_NUMBER 7
#define RTS_PIN_NUMBER 5

void uart_boot(void) {
  UARTE->PSEL.RTS = RTS_PIN_NUMBER;
  UARTE->PSEL.TXD = TX_PIN_NUMBER;
  UARTE->PSEL.CTS = CTS_PIN_NUMBER;
  UARTE->PSEL.RXD = RX_PIN_NUMBER;

  UARTE->BAUDRATE = 0x01D60000; // 115200

  // no parity, 1 stop bit, flow control
  UARTE->CONFIG = 1;
  UARTE->ENABLE = 8;

  memset(s_rx_recv_buf, 0x0, sizeof(s_rx_recv_buf));
  UARTE->RXD.PTR = (uint32_t)s_rx_recv_buf;
  UARTE->RXD.MAXCNT = 1;

  prv_enable_nvic(2);
  UARTE->INTENSET |= (1 << 4);
  UARTE->TASKS_STARTRX = 1;
}

void uart_boot3(void) {
  UART->PSEL.RTS = RTS_PIN_NUMBER;
  UART->PSEL.TXD = TX_PIN_NUMBER;
  UART->PSEL.CTS = CTS_PIN_NUMBER;
  UART->PSEL.RXD = RX_PIN_NUMBER;

  UART->BAUDRATE = 0x01D60000; // 115200
  // UART->

  UART->CONFIG = 1;
  UART->ENABLE = 4;
  prv_enable_nvic(2);
  UARTE->INTENSET |= (1 << 4);
  UARTE->TASKS_STARTRX = 1;
}

static inline void tx_byte(uint8_t c) {
  *(volatile uint32_t*) UART->EVENTS_TXDRDY = 0;
  UART->TXD = c;
}

void uart_tx_blocking3(void *buf, size_t buf_len) {
  int t = 0;
  //while (!UARTE->EVENTS_ENDTX && !UARTE->EVENTS_TXSTOPPED) {  }
  *(volatile uint32_t*) UART->EVENTS_TXDRDY = 0;
  *(volatile uint32_t*) UART->TASKS_STARTTX = 1;
  while (t < buf_len) {
    while (UART->EVENTS_TXDRDY != 1) {};
    tx_byte(((uint8_t *) buf)[t++]);
  }
  while (UART->EVENTS_TXDRDY != 1) {}; 
  *(volatile uint32_t*) UART->TASKS_STOPTX = 1; 
}

void Irq2_Handler3(void) {
  if (UART->EVENTS_ERROR != 1) {
    UART->EVENTS_ERROR = 0;

    char c = s_rx_recv_buf[0];
    uart_byte_received_from_isr_cb(c);

    // re-arm
    UART->TASKS_STARTRX = 1;
  }
}

void uart_tx_blocking(void *buf, size_t buf_len) {
  UARTE->EVENTS_ENDTX = 0;
  UARTE->EVENTS_TXSTOPPED = 0;

  UARTE->TXD.PTR = (uint32_t)buf;
  UARTE->TXD.MAXCNT = buf_len;

  UARTE->TASKS_STARTTX = 1;

  while (!UARTE->EVENTS_ENDTX && !UARTE->EVENTS_TXSTOPPED) {  }
}

void Irq2_Handler(void) {
  if (UARTE->EVENTS_ENDRX != 0) {
    UARTE->EVENTS_ENDRX = 0;

    char c = s_rx_recv_buf[0];
    uart_byte_received_from_isr_cb(c);

    // re-arm
    UARTE->TASKS_STARTRX = 1;
  }
}
