#include "vm.h"
#include "lib/chunk.h"
#include "lib/memory.h"
#include "lox.h"
#include <stdarg.h>
#include <stdio.h>

void lox_vm_init(lox_vm *vm) {
    lox_vm_resetstack(vm);
    vm->cur_chunk = NULL;
    vm->ip = NULL;
}
void lox_vm_free(lox_vm *vm, struct memory_tracker *tracker) {}

void lox_vm_resetstack(lox_vm *vm) {
    vm->stack_top = vm->stack;
}
void lox_vm_reporterror(lox_vm *vm, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    size_t instruction = vm->ip - vm->cur_chunk->code - 1;
    int line = vm->cur_chunk->lines[instruction];
    fprintf(stderr, "[line %d] in script\n", line);
    lox_vm_resetstack(vm);
}
lox_value lox_vm_popvalue(lox_vm *vm) {
    if (vm->stack_top - vm->stack == 0)
        return 0;
    vm->stack_top--;
    return *vm->stack_top;
}
lox_error_t lox_vm_pushvalue(lox_vm *vm, lox_value value) {
    if (vm->stack_top - vm->stack == STACK_MAX) {
        return LOX_ERROR_STACK_OVERFLOW;
    }
    *vm->stack_top = value;
    vm->stack_top++;
    return LOX_ERROR_OK;
}
lox_error_t lox_vm_interpret(lox_vm *vm) {
#define read_byte() *(vm->ip++)
    for (;;) {
        int op = read_byte();
        switch ((lox_opcode_t)op) {
        case OP_RETURN:
            return LOX_ERROR_OK;
        }
    }
#undef read_byte
}
