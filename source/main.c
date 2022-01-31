#include <stdio.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"

int main()
{
	Chunk chunk;
	chunk_init(&chunk);

	const size_t index = chunk_write_constant(&chunk, 12.5);
	chunk_write(&chunk, OP_CONSTANT);
	chunk_write(&chunk, index);

	chunk_write(&chunk, OP_RETURN);

	debug_chunk(&chunk, "test");

	chunk_free(&chunk);

	return 0;
}