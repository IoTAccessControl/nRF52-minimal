#ifndef SECURE_CHANNEL_H_
#define SECURE_CHANNEL_H_


int init_secure_channel();

int secure_putc(char c);
char secure_getc(void);

void debug_log(const char *fmt, ...);

#endif
