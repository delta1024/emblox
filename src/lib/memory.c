#include "memory.h"
#include <stdlib.h>

static inline void update_memory(struct memory_tracker *tracker, size_t size) {
    tracker->cur_usage += size;
    tracker->total_usage += size;
}
void *lox_malloc(struct memory_tracker *tracker, size_t size) {
    void *ptr = malloc(size);
    if (ptr != NULL)
        update_memory(tracker, size);
    return ptr;
}

void lox_free(struct memory_tracker *tracker, void *ptr, size_t size) {
    tracker->cur_usage -= size;
    free(ptr);
}

void *lox_calloc(struct memory_tracker *tracker, size_t nmemb, size_t size) {
    void *ptr = calloc(nmemb, size);
    if (ptr != NULL)
        update_memory(tracker, size);
    return ptr;
}

void *lox_realloc(struct memory_tracker *tracker, void *mem, size_t new_size,
                  size_t old_size) {
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
void *lox_reallocarray(struct memory_tracker *tracker, void *mem, size_t nmemb,
                       size_t old_nmemb, size_t size) {
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
