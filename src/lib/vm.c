#include "vm.h"
#include "chunk.h"
#include "lox.h"
#include "value.h"
#include <stdio.h>
#ifdef DEBUG_TRACE_EXECUTION
#include "debug.h"
#endif /* ifdef DEBUG_TRACE_EXECUTION */

void lox_vm_init(lox_vm *vm) {
    vm->chunk = NULL;
    vm->ip    = NULL;
}
void lox_vm_free(lox_vm *vm, lox_memory *allocator) {}
void lox_vm_resetstack(lox_vm *vm) {}

lox_error lox_vm_run(lox_vm *vm) {
#define read_byte() (*vm->ip++)
#define read_constant() (vm->chunk->constants.entries[read_byte()])
    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        lox_debug_dissasemble_instruction(vm->chunk, vm->ip - vm->chunk->code);
#endif /* ifdef DEBUG_TRACE_EXECUTION */
        uint8_t byte = read_byte();
        switch ((op_code_t)byte) {
        case OP_CONSTANT: {
            value val = read_constant();
            lox_print_value(val);
            printf("\n");
            break;
        }
        case OP_RETURN:
            return LOX_ERROR_OK;
        }
    }
#undef read_byte
#undef read_constant
}
