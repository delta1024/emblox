#include "vm.h"
#include "chunk.h"
#include "lox.h"
#include "value.h"
#include <stdio.h>
#ifdef DEBUG_TRACE_EXECUTION
#include "common.h"
#include "debug.h"
#endif /* ifdef DEBUG_TRACE_EXECUTION */

void lox_vm_init(lox_vm *vm) {
    vm->chunk = NULL;
    vm->ip    = NULL;
}
void lox_vm_free(lox_vm *vm, lox_memory *allocator) {}
void lox_vm_resetstack(lox_vm *vm) {
    vm->stack_top = vm->stack;
}
void lox_vm_reporterror(lox_vm *vm, const char *message) {
    fprintf(stderr, "Error: %s\n", message);

    int offset   = vm->ip - vm->chunk->code;
    uint8_t line = vm->chunk->lines[offset];
    fprintf(stderr, "[line %d] in script\n", line);
}
void lox_vm_push(lox_vm *vm, lox_value value) {
    if (vm->stack_top - vm->stack == STACK_MAX) {
        lox_vm_reporterror(vm, "Stack Overflow");
        return;
    }
    *vm->stack_top = value;
    vm->stack_top++;
}
lox_value lox_vm_pop(lox_vm *vm) {
    vm->stack_top--;
    return *vm->stack_top;
}
lox_value lox_vm_peek(lox_vm *vm, int span) {
    return *(vm->stack_top - 1 - span);
}

lox_error lox_vm_run(lox_vm *vm) {
#define read_byte() (*vm->ip++)
#define read_constant() (vm->chunk->constants.entries[read_byte()])
    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printf("\t");
        for (lox_value *slot = vm->stack; slot < vm->stack_top; slot++) {
            printf("[ ");
            lox_value_print(*slot);
            printf(" ]");
        }
        printf("\n");
        lox_debug_dissasembleinstruction(vm->chunk, vm->ip - vm->chunk->code);
#endif /* ifdef DEBUG_TRACE_EXECUTION */

        uint8_t byte = read_byte();
        switch ((op_code_t)byte) {
        case OP_CONSTANT: {
            lox_value val = read_constant();
            lox_vm_push(vm, val);
            break;
        }
        case OP_RETURN:
            lox_value_print(lox_vm_pop(vm));
            printf("\n");
            return LOX_ERROR_OK;
        }
    }
#undef read_byte
#undef read_constant
}
