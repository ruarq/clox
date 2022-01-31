#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"

typedef enum
{
	OP_RETURN
} OpCode;

typedef struct
{
	size_t count;
	size_t capacity;
	uint8_t *code;
} Chunk;

#endif