#include <stdio.h>

#include "common.h"
#include "vm.h"
#include "chunk.h"
#include "debug.h"

int main()
{
	vm_init();

	Chunk chunk;
	chunk_init(&chunk);

	const size_t indexA = chunk_write_constant(&chunk, 12.5);
	const size_t indexB = chunk_write_constant(&chunk, 12.5);

	chunk_write(&chunk, OP_CONSTANT, 0);
	chunk_write(&chunk, indexA, 0);
	chunk_write(&chunk, OP_CONSTANT, 0);
	chunk_write(&chunk, indexB, 0);
	chunk_write(&chunk, OP_ADD, 1);
	chunk_write(&chunk, OP_NEGATE, 1);
	chunk_write(&chunk, OP_RETURN, 1);

	vm_interpret(&chunk);

	chunk_free(&chunk);

	vm_free();
	return 0;
}