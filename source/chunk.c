#include "chunk.h"

void chunk_init(Chunk *chunk)
{
	chunk->size = 0;
	chunk->capacity = 0;
	chunk->code = NULL;
	chunk_realloc(chunk, 1);
}

void chunk_write(Chunk *chunk, const uint8_t byte)
{
	if (chunk->size >= chunk->capacity)
	{
		chunk_realloc(chunk, chunk->capacity * 2);
	}

	chunk->code[chunk->size++] = byte;
}

void chunk_realloc(Chunk *chunk, const size_t capacity)
{
	chunk->code = realloc(chunk->code, capacity * sizeof(uint8_t));
	chunk->capacity = capacity;
}

void chunk_free(Chunk *chunk)
{
	free(chunk->code);
	chunk_init(chunk);
}