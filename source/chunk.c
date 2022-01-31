#include "chunk.h"

void chunk_init(Chunk *chunk)
{
	chunk->size = 0;
	chunk->capacity = 0;
	chunk->code = NULL;
	chunk->lines = NULL;
	chunk_realloc(chunk, 1);

	value_array_init(&chunk->constants);
}

void chunk_write(Chunk *chunk, const uint8_t byte, const size_t line)
{
	if (chunk->size >= chunk->capacity)
	{
		chunk_realloc(chunk, chunk->capacity * 2);
	}

	chunk->code[chunk->size] = byte;
	chunk->lines[chunk->size] = line;
	++chunk->size;
}

size_t chunk_write_constant(Chunk *chunk, const Value constant)
{
	value_array_write(&chunk->constants, constant);
	return chunk->constants.size - 1;
}

void chunk_realloc(Chunk *chunk, const size_t capacity)
{
	chunk->code = realloc(chunk->code, capacity * sizeof(uint8_t));
	chunk->lines = realloc(chunk->lines, capacity * sizeof(size_t));
	chunk->capacity = capacity;
}

void chunk_free(Chunk *chunk)
{
	value_array_free(&chunk->constants);

	free(chunk->code);
	chunk_init(chunk);
}