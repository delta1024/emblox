#ifndef _LOX_VM_H
#define _LOX_VM_H
#include "chunk.h"
#include "lox.h"
#include "memory.h"

typedef struct lox_vm {
    lox_chunk *chunk;
    uint8_t *ip;
} lox_vm;

void lox_vm_init(lox_vm *vm);
void lox_vm_free(lox_vm *vm, lox_memory *allocator);
void lox_vm_resetstack(lox_vm *vm);
lox_error lox_vm_run(lox_vm *vm);

#endif // !_LOX_VM_H
