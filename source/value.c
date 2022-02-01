#include "value.h"

void value_print(const Value value)
{
	printf("%g", value.as.number);
}

void value_array_init(ValueArray *va)
{
	va->size = 0;
	va->capacity = 0;
	va->values = NULL;
	value_array_realloc(va, 1);
}

void value_array_write(ValueArray *va, const Value value)
{
	if (va->size >= va->capacity)
	{
		value_array_realloc(va, va->capacity * 2);
	}

	va->values[va->size++] = value;
}

void value_array_realloc(ValueArray *va, const size_t capacity)
{
	va->values = realloc(va->values, capacity * sizeof(Value));
	va->capacity = capacity;
}

void value_array_free(ValueArray *va)
{
	free(va->values);
	value_array_init(va);
}