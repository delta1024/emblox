#ifndef _LOX_MEMORY_H
#define _LOX_MEMORY_H
#include <stddef.h>

struct memory_tracker {
    size_t cur_usage, total_usage;
};
void *lox_malloc(size_t size, struct memory_tracker *tracker);
void lox_free(void *ptr, size_t size, struct memory_tracker *tracker);
void *lox_calloc(size_t nmemb, size_t size, struct memory_tracker *tracker);
void *lox_realloc(void *ptr, size_t new_size, size_t old_size,
                  struct memory_tracker *tracker);
void *lox_reallocarray(void *ptr, size_t nmemb, size_t old_nmemb, size_t size,
                       struct memory_tracker *tracker);
#define arraysize(ty, num) (sizeof(ty) * (num))
#endif // !_LOX_MEMORY_H
