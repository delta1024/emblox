#ifndef _LOX_DEBUG_H
#define _LOX_DEBUG_H

#include "chunk.h"

void lox_debug_dissasemblechunk(lox_chunk *chunk, const char *name);
int lox_debug_dissasembleinstruction(lox_chunk *chunk, int offset);

#endif // !_LOX_DEBUG_H
