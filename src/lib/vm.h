#ifndef _LOX_VM_H
#define _LOX_VM_H
#include "chunk.h"
#include "common.h"
#include "memory.h"
#include "value.h"
#include <lox.h>

#define STACK_MAX UINT8_MAX

typedef struct lox_vm {
    lox_value stack[STACK_MAX];
    lox_value *stack_top;
    lox_chunk *cur_chunk;
    uint8_t *ip;
    struct memory_tracker *tracker;
} lox_vm;

void lox_vm_init(struct memory_tracker *tracker, lox_vm *vm);
void lox_vm_free(lox_vm *vm);

lox_error_t lox_vm_interpret(lox_vm *vm);
void lox_vm_reporterror(lox_vm *vm, const char *format, ...);

void lox_vm_resetstack(lox_vm *vm);
lox_value lox_vm_popvalue(lox_vm *vm);
lox_error_t lox_vm_pushvalue(lox_vm *vm, lox_value value);

#endif // !_LOX_VM_H
