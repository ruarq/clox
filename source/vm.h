#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "chunk.h"

typedef struct
{
	Chunk *chunk;
} VM;

typedef enum
{
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
} InterpretResult;

void vm_init();
InterpretResult vm_interpret(Chunk *chunk);
void vm_free();

#endif