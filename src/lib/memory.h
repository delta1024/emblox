#ifndef _LOX_MEMORY_H
#define _LOX_MEMORY_H
#include <lox.h>
typedef struct lox_memory {
	void *ud;
	lox_alloc_fn f;
} lox_memory;
#endif // !_LOX_MEMORY_H
