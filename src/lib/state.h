#ifndef _LOX_STATE_H
#define _LOX_STATE_H
#include <lox.h>
#include "common.h"

typedef struct lox_memory {
	void *ud;
	lox_alloc_fn f;
} lox_memory;

struct lox_state {
	lox_memory memory;
	bool user_allocator;
};


#endif // !_LOX_STATE_H
