#ifndef CLOX_VM_H
#define CLOX_VM_H

#include <stdarg.h>

#include "chunk.h"
#include "compiler.h"
#include "debug.h"

#define VM_STACK_MAX 1 << 8

typedef struct
{
	Chunk *chunk;
	uint8_t *ip;

	Value stack[VM_STACK_MAX];
	Value *stack_top;
} VM;

typedef enum
{
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
} InterpretResult;

void vm_init();
void vm_free();

InterpretResult vm_interpret(const char *source);
InterpretResult vm_run();

void vm_reset_stack();
void vm_stack_push(const Value value);
Value vm_stack_pop();

void vm_runtime_error(const char *fmt, ...);

#endif