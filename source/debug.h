#ifndef CLOX_DEBUG_H
#define CLOX_DEBUG_H

#include <stdio.h>

#include "chunk.h"
#include "value.h"

void debug_chunk(const Chunk *chunk, const char *name);

/**
 * @brief debug a instruction located in a chunk
 * @param chunk the chunk
 * @param index the index of the instruction
 * @return index of the next instruction
 */
size_t debug_instruction(const Chunk *chunk, const size_t index);

#endif