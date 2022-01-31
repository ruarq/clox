#include <stdio.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"

int main()
{
	Chunk chunk;
	chunk_init(&chunk);

	const size_t index = chunk_write_constant(&chunk, 12.5);
	chunk_write(&chunk, OP_CONSTANT, 0);
	chunk_write(&chunk, index, 0);

	chunk_write(&chunk, OP_RETURN, 1);

	debug_chunk(&chunk, "test");

	chunk_free(&chunk);

	return 0;
}