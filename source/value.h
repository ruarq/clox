#ifndef CLOX_VALUE_H
#define CLOX_VALUE_H

#include <stdlib.h>
#include <stdio.h>

#include "common.h"

typedef enum
{
	VAL_NIL,
	VAL_BOOL,
	VAL_NUM
} ValueType;

typedef struct
{
	ValueType type;

	union
	{
		bool boolean;
		double number;
	} as;
} Value;

void value_print(const Value value);

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

#define value_create_nil()		((Value){.type = VAL_NIL, {}})
#define value_create_bool(x)	((Value){.type = VAL_BOOL, {.boolean = x}})
#define value_create_number(x)	((Value){.type = VAL_NUM, {.number = x}})

#define value_is_nil(x)			((x).type == VAL_NIL)
#define value_is_bool(x)		((x).type == VAL_BOOL)
#define value_is_number(x)		((x).type == VAL_NUM)

#endif