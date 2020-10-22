#pragma once
#define DEBUG

void debug_log(const char *fmt, ...);

#ifdef DEBUG
#define DEBUG_LOG(...)                            \
  do {                                              \
    debug_log(__VA_ARGS__);                       \
  } while (0)
#else
#define DEBUG_LOG(...)
#endif
