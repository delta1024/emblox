#include "memory.h"
#include <stdlib.h>

static inline void update_memory(struct memory_tracker *tracker, size_t size) {
    tracker->cur_usage   += size;
    tracker->total_usage += size;
}
void *lox_malloc(size_t size, struct memory_tracker *tracker) {
    void *ptr = malloc(size);
    if (ptr != NULL)
        update_memory(tracker, size);
    return ptr;
}

void lox_free(void *ptr, size_t size, struct memory_tracker *tracker) {
    tracker->cur_usage -= size;
    free(ptr);
}

void *lox_calloc(size_t nmemb, size_t size, struct memory_tracker *tracker) {
    void *ptr = calloc(nmemb, size);
    if (ptr != NULL)
        update_memory(tracker, size);
    return ptr;
}

void *lox_realloc(void *mem, size_t new_size, size_t old_size,
                  struct memory_tracker *tracker) {
    void *ptr = realloc(mem, new_size);
    if (ptr != NULL) {
        if (new_size > old_size) {
            update_memory(tracker, new_size - old_size);
        } else {
            tracker->cur_usage -= (old_size - new_size);
        }
    }
    return ptr;
}
void *lox_reallocarray(void *mem, size_t nmemb, size_t old_nmemb, size_t size,
                       struct memory_tracker *tracker) {
    void *ptr = reallocarray(mem, nmemb, size);
    if (ptr != NULL) {
        if (nmemb > old_nmemb) {
            update_memory(tracker, (nmemb - old_nmemb) * size);
        } else {
            tracker->cur_usage -= ((old_nmemb - nmemb) * size);
        }
    }
    return ptr;
}
