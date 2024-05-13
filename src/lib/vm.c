#include "vm.h"
#include "chunk.h"
#include "memory.h"
#include "value.h"
#include <lox.h>
#include <stdarg.h>
#include <stdio.h>
#ifdef DEBUG_TRACE_EXECUTION
#include "debug.h"
#endif /* ifdef DEBUG_TRACE_EXECUTION */

static inline bool check_err(lox_error_t err) {
    return err == LOX_ERROR_OK;
}
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
lox_value lox_vm_peekvalue(lox_vm *vm, int span) {
    return *(vm->stack_top - 1 - span);
}
lox_error_t lox_vm_interpret(lox_vm *vm) {
#define read_byte() *(vm->ip++)
#define read_constant() (vm->cur_chunk->constants.entries[read_byte()])
    for (;;) {

        #ifdef DEBUG_TRACE_EXECUTION
        printf("         ");
        for (lox_value *slot = vm->stack; slot < vm->stack_top; slot++) {
            printf("[ ");
            lox_value_print(*slot);
            printf(" ]");
        }
        printf("\n");
        lox_debug_dissasemble_instruction(vm->cur_chunk, vm->ip - vm->cur_chunk->code);
        #endif /* ifdef DEBUG_TRACE_EXECUTION */

        int op = read_byte();
        switch ((lox_opcode_t)op) {
            case OP_CONSTANT:  {
                lox_value val = read_constant();
                lox_error_t res = lox_vm_pushvalue(vm, val);
                if (!check_err(res)) {
                    return res;
                }
                break;
            }
            case OP_RETURN: {
                lox_value val = lox_vm_popvalue(vm);
                lox_value_print(val);
                printf("\n");
                return LOX_ERROR_OK;
            }
        }
    }
#undef read_byte
#undef read_constant
}
