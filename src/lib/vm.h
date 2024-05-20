#ifndef _LOX_VM_H
#define _LOX_VM_H
#include "chunk.h"
#include "common.h"
#include "lox.h"
#include "memory.h"
#include "value.h"

#define STACK_MAX UINT8_MAX
typedef struct lox_vm {
    lox_value stack[STACK_MAX];
    lox_value *stack_top;
    lox_chunk *chunk;
    uint8_t *ip;
} lox_vm;

void lox_vm_init(lox_vm *vm);
void lox_vm_free(lox_vm *vm, lox_memory *allocator);
void lox_vm_resetstack(lox_vm *vm);
void lox_vm_reporterror(lox_vm *vm, const char *message);
void lox_vm_push(lox_vm *vm, lox_value value);
lox_value lox_vm_pop(lox_vm *vm);
lox_value lox_vm_peek(lox_vm *vm, int span);
lox_error lox_vm_run(lox_vm *vm);

#endif // !_LOX_VM_H
