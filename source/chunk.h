#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include <stdlib.h>

#include "common.h"

typedef enum
{
	OP_RETURN
} OpCode;

typedef struct
{
	size_t size;
	size_t capacity;
	uint8_t *code;
} Chunk;

void chunk_init(Chunk *chunk);
void chunk_write(Chunk *chunk, const uint8_t byte);
void chunk_realloc(Chunk *chunk, const size_t capacity);

#endif