#ifndef LOX_H
#define LOX_H
#include <stddef.h>
typedef double lox_number;
typedef enum lox_error {
	LOX_ERROR_OK,
	LOX_ERROR_COMPILE,
	LOX_ERROR_INTERPRET,
} lox_error;
typedef void * (*lox_alloc_fn) (void *ud,
				void *ptr,
				size_t osize,
				size_t nsize);
typedef struct lox_state lox_state;

lox_state *lox_newstate(lox_alloc_fn f, void *ud);
void lox_close(lox_state *state);

void lox_pushnumber(lox_state *state, lox_number n);
lox_number lox_tonumber(lox_state *state, int index);

lox_error lox_loadbuffer(lox_state *state, const char *buff,
			 size_t sz, const char *name);
lox_error lox_pcall(lox_state *state, int nargs);


#endif // !LOX_H
