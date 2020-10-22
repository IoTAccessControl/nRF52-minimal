#include "shell_port.h"
#include <stddef.h>
#include <stdbool.h>

static volatile struct {
  size_t read_idx;
  size_t num_bytes;
  char buf[64];
} s_uart_buffer = {
  .num_bytes = 0,
};

void uart_byte_received_from_isr_cb(char c) {
  if (s_uart_buffer.num_bytes >= sizeof(s_uart_buffer.buf)) {
    return; // drop, out of space
  }

  s_uart_buffer.buf[s_uart_buffer.read_idx] = c;
  s_uart_buffer.num_bytes++;
}