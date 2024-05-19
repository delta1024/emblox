#include "vm.h"
#include "chunk.h"
#include "lox.h"

void lox_vm_init(lox_vm *vm) {
    vm->chunk = NULL;
    vm->ip    = NULL;
}
void lox_vm_free(lox_vm *vm, lox_memory *allocator) {}
void lox_vm_resetstack(lox_vm *vm) {}

lox_error lox_vm_run(lox_vm *vm) {
#define read_byte() (*vm->ip++)
    for (;;) {
        uint8_t byte = read_byte();
        switch ((op_code_t)byte) {
        case OP_RETURN:
            return LOX_ERROR_OK;
        }
    }
#undef read_byte
}
