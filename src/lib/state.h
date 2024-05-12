#ifndef _LOX_STATE_H_
#define _LOX_STATE_H_
#include "chunk.h"
#include "memory.h"
#include "vm.h"
// IWYU pragma: private;
struct lox_state {
    lox_vm vm;
    lox_chunk chunk;
    struct memory_tracker memory;
    lox_error_t last_error;
};
#endif // !_LOX_STATE_H_
