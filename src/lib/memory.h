#ifndef _LOX_MEMORY_H
#define _LOX_MEMORY_H
#include <lox.h>
typedef struct lox_memory {
    void *ud;
    lox_alloc_fn f;
} lox_memory;

#define lox_allocate(allocator, ptr, osize, nsize) \
((allocator)->f((allocator)->ud, (ptr), (osize), (nsize)))
#endif // !_LOX_MEMORY_H
