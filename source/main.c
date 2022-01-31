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

	const size_t index = chunk_write_constant(&chunk, 12.5);
	chunk_write(&chunk, OP_CONSTANT, 0);
	chunk_write(&chunk, index, 0);
	chunk_write(&chunk, OP_NEGATE, 1);
	chunk_write(&chunk, OP_RETURN, 1);

	vm_interpret(&chunk);

	chunk_free(&chunk);

	vm_free();
	return 0;
}