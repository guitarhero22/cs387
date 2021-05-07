#ifndef __UTILS_H__
#define __UTILS_H__

#ifdef _DEBUG
#define errlog(a, ...) fprintf(stderr, a, ##__VA_ARGS__), fflush(stderr)
#else
#define errlog(a, ...)
#endif

#include <stdint.h>

typedef uint8_t byte_;

#define TIME_FMT "%Y-%m-%d.%X"
#define TOMBSTONE_BYTE 0xFF

#endif // __UTILS_H__