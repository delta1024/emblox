#include "lox.h"
#include "chunk.h"
#include "state.h" // IWYU pragma: keep
#include "vm.h"
#include <assert.h>
#include <stdlib.h>
typedef struct lox_allocator {
	size_t bytes_allocated;
	size_t bytes_freed;
} lox_allocator;

void *lox_alloc_inner(void *ud, void *ptr, size_t osize, size_t nsize) {
	if (ud == NULL) {
		free(ptr);
		return NULL;
	}
	lox_allocator *allocator = (lox_allocator*)ud;
	if (nsize == 0) {
		free(ptr);
		allocator->bytes_freed += osize;
		return NULL;
	}
	ptr = realloc(ptr, nsize);
	if (ptr == NULL) 
		return NULL;
	if (osize > nsize) {
		allocator->bytes_freed += osize - nsize;
	} else {
		allocator->bytes_allocated += nsize - osize;
	}
	return ptr;
}

lox_state *lox_newstate(lox_alloc_fn f, void *ud) {
	lox_state *state;
	if (f == NULL) {
		lox_allocator allocator, *ptr;
		allocator = (lox_allocator){0,0};
		ptr = lox_alloc_inner(&allocator, NULL, 0, sizeof(lox_allocator));
		if (ptr == NULL) return NULL;
		*ptr = allocator;
		state = lox_alloc_inner(ptr, NULL, 0, sizeof(lox_state));
		if (state == NULL) {
			lox_alloc_inner(NULL, ptr, sizeof(lox_allocator), 0);
			return NULL;
		}
		state->memory.ud = ptr;
		state->memory.f = lox_alloc_inner;
	} else {
		state = f(ud, NULL, 0, sizeof(lox_state));
		if (state == NULL) return NULL;
		state->memory.ud = ud;
		state->memory.f = f;
	}
	return state;
}
void lox_close(lox_state *state) {
	lox_memory memory = state->memory;
	memory.f(memory.ud, state, sizeof(lox_state), 0);
	if (state->user_allocator) {
		return;
	}
	lox_chunk_free(&state->chunk, &memory);
	lox_allocator *allocator = (lox_allocator*)memory.ud;
	assert(allocator->bytes_allocated - sizeof(lox_allocator) == allocator->bytes_freed);
	memory.f(NULL, allocator, sizeof(lox_allocator), 0);
}
lox_error lox_loadbuffer(lox_state *state, const char *buff, size_t sz, const char *name) {
	lox_chunk_writebyte(&state->chunk,OP_RETURN, 0, &state->memory);
	return LOX_ERROR_OK;

}
lox_error lox_pcall(lox_state *state, int nargs) {
	lox_error err;

	state->vm.chunk = &state->chunk;
	state->vm.ip = state->chunk.code;
	lox_vm_resetstack(&state->vm);
	
	if ((err = lox_vm_run(&state->vm)) != LOX_ERROR_OK) {
		return err;
	}
	return LOX_ERROR_OK;
}
