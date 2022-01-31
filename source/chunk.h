#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include <stdlib.h>

#include "common.h"
#include "value.h"

typedef enum
{
	OP_CONSTANT,
	OP_NEGATE,
	OP_RETURN
} OpCode;

typedef struct
{
	size_t size;
	size_t capacity;
	uint8_t *code;
	size_t *lines;

	ValueArray constants;
} Chunk;

void chunk_init(Chunk *chunk);
void chunk_write(Chunk *chunk, const uint8_t byte, const size_t line);

/**
 * @brief write a constant to the chunk
 * @param chunk the chunk
 * @param value the constant
 * @return the index where the constant was stored
 */
size_t chunk_write_constant(Chunk *chunk, const Value constant);
void chunk_realloc(Chunk *chunk, const size_t capacity);
void chunk_free(Chunk *chunk);

#endif