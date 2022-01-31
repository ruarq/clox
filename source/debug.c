#include "debug.h"

void debug_chunk(Chunk *chunk, const char *name)
{
	printf("=== %s ===\n", name);

	for (size_t i = 0; i < chunk->size;)
	{
		i = debug_instruction(chunk, i);
	}
}

size_t debug_instruction(Chunk *chunk, const size_t index)
{
	printf("%04ld ", index);
	const uint8_t instruction = chunk->code[index];
	switch (instruction)
	{
		case OP_RETURN:
			printf("OP_RETURN\n");
			return index + 1;

		default:
			printf("unknown OpCode %d", instruction);
			return index + 1;
	}
}