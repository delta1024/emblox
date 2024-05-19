#ifndef _LOX_STATE_H
#define _LOX_STATE_H
#include "chunk.h"
#include "common.h"
#include "memory.h"
#include "vm.h"
#include <lox.h>

struct lox_state {
    lox_memory memory;
    bool user_allocator;
    lox_chunk chunk;
    lox_vm vm;
};

#endif // !_LOX_STATE_H
