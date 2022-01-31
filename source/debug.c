#include "debug.h"

void instruction_simple(const char *name)
{
	printf("%-16s\n", name);
}

size_t instruction_constant(const char *name, const Chunk *chunk, const size_t index)
{
	const uint8_t constant = chunk->code[index + 1];
	printf("%-16s %4d '", name, constant);
	value_print(chunk->constants.values[constant]);
	printf("'\n");

	return index + 2;
}

void debug_chunk(const Chunk *chunk, const char *name)
{
	printf("=== %s ===\n", name);

	for (size_t i = 0; i < chunk->size;)
	{
		i = debug_instruction(chunk, i);
	}
}

size_t debug_instruction(const Chunk *chunk, const size_t index)
{
	printf("%04ld ", index);

	if (index > 0 && chunk->lines[index] == chunk->lines[index - 1])
	{
		printf("   | ");
	}
	else
	{
		printf("%4ld ", chunk->lines[index]);
	}

	const uint8_t instruction = chunk->code[index];
	switch (instruction)
	{
		case OP_CONSTANT:
			return instruction_constant("OP_CONSTANT", chunk, index);

		case OP_RETURN:
			instruction_simple("OP_RETURN");
			return index + 1;

		default:
			printf("unknown OpCode %d", instruction);
			return index + 1;
	}
}