#ifndef CLOX_VALUE_H
#define CLOX_VALUE_H

#include <stdlib.h>

#include "common.h"

typedef double Value;

typedef struct
{
	size_t size;
	size_t capacity;
	Value *values;
} ValueArray;

void value_array_init(ValueArray *va);
void value_array_write(ValueArray *va, const Value value);
void value_array_realloc(ValueArray *va, const size_t capacity);
void value_array_free(ValueArray *va);

#endif