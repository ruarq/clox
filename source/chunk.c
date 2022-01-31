#include "chunk.h"

void chunk_init(Chunk *chunk)
{
	chunk->size = 0;
	chunk->capacity = 0;
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
	// reallocate buffer
	chunk->code = realloc(chunk->code, capacity * sizeof(uint8_t));

	// assign new capacity
	chunk->capacity = capacity;
}