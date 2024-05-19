#ifndef _LOX_STATE_H
#define _LOX_STATE_H
#include <lox.h>
#include "memory.h"
#include "common.h"
#include "chunk.h"
#include "vm.h"


struct lox_state {
	lox_memory memory;
	bool user_allocator;
	lox_chunk chunk;
	lox_vm vm;
};


#endif // !_LOX_STATE_H
