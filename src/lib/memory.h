#ifndef _LOX_MEMORY_H
#define _LOX_MEMORY_H
#include <stddef.h>

struct memory_tracker {
    size_t cur_usage, total_usage;
};
void *lox_malloc(struct memory_tracker *tracker, size_t size);
void lox_free(struct memory_tracker *tracker, void *ptr, size_t size);
void *lox_calloc(struct memory_tracker *tracker, size_t nmemb, size_t size);
void *lox_realloc(struct memory_tracker *tracker, void *ptr, size_t new_size,
                  size_t old_size);
void *lox_reallocarray(struct memory_tracker *tracker, void *ptr, size_t nmemb,
                       size_t old_nmemb, size_t size);
#define arraysize(ty, num) (sizeof(ty) * (num))
#endif // !_LOX_MEMORY_H
