#include <stdio.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"

int main()
{
	Chunk chunk;
	chunk_init(&chunk);
	chunk_write(&chunk, OP_RETURN);
	debug_chunk(&chunk, "test");
	chunk_free(&chunk);
	return 0;
}