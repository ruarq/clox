#include "vm.h"

#define VM_BINARY_OP(vtype, op) \
	do \
	{ \
		if (!value_is_number(vm.stack_top[-1]) || !value_is_number(vm.stack_top[-2])) \
		{ \
			vm_runtime_error("Operands must be numbers"); \
			return INTERPRET_RUNTIME_ERROR; \
		} \
		const double b = vm_stack_pop().as.number; \
		const double a = vm_stack_pop().as.number; \
		vm_stack_push(vtype(a op b)); \
	} \
	while (false)

VM vm;

void vm_init()
{
	vm_reset_stack();
}

void vm_free()
{

}

InterpretResult vm_interpret(const char *source)
{
	Chunk chunk;
	chunk_init(&chunk);

	if (!compile(source, &chunk))
	{
		chunk_free(&chunk);
		return INTERPRET_COMPILE_ERROR;
	}

	vm.chunk = &chunk;
	vm.ip = chunk.code;

	InterpretResult result = vm_run();

	chunk_free(&chunk);
	return result;
}

InterpretResult vm_run()
{
	// I don't like this. Looping over the chunk feels better and safer.
	while (true)
	{
		const uint8_t instruction = *(vm.ip++);

		#if defined(DEBUG_TRACE_EXECUTION)
		printf("        ");
		for (Value *slot = vm.stack; slot != vm.stack_top; ++slot)
		{
			printf("[ ");
			value_print(*slot);
			printf(" ]");
		}
		printf("\n");
		debug_instruction(vm.chunk, vm.ip - vm.chunk->code);
		#endif

		switch (instruction)
		{
			case OP_CONSTANT:
			{
				const Value constant = vm.chunk->constants.values[*vm.ip++];
				vm_stack_push(constant);
			}
			break;

			case OP_ADD:	VM_BINARY_OP(value_create_number, +);	break;
			case OP_SUB:	VM_BINARY_OP(value_create_number, -);	break;
			case OP_MUL:	VM_BINARY_OP(value_create_number, *);	break;
			case OP_DIV:	VM_BINARY_OP(value_create_number, /);	break;

			case OP_NEGATE:
				if (!value_is_number(vm.stack_top[-1]))
				{
					vm_runtime_error("Operand must be a number");
					return INTERPRET_RUNTIME_ERROR;
				}

				vm_stack_push(value_create_number(-vm_stack_pop().as.number));
				break;

			case OP_RETURN:
				value_print(vm_stack_pop());
				printf("\n");
				return INTERPRET_OK;
		}
	}
}

void vm_reset_stack()
{
	vm.stack_top = vm.stack;
}

void vm_stack_push(const Value value)
{
	*vm.stack_top = value;
	++vm.stack_top;
}

Value vm_stack_pop()
{
	--vm.stack_top;
	return *vm.stack_top;
}

void vm_runtime_error(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fputs("\n", stderr);

	const size_t instruction = vm.ip - vm.chunk->code - 1;
	const size_t line = vm.chunk->lines[instruction];
	fprintf(stderr, "[line %ld] in script\n", line);
	vm_reset_stack();
}