#include "vm.h"

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

			case OP_ADD:
				vm_stack_push(vm_stack_pop() + vm_stack_pop());
				break;

			case OP_SUB:
				vm_stack_push(-vm_stack_pop() + vm_stack_pop());
				break;

			case OP_MUL:
				vm_stack_push(vm_stack_pop() * vm_stack_pop());
				break;

			case OP_DIV:
			{
				const Value b = vm_stack_pop();
				const Value a = vm_stack_pop();
				vm_stack_push(a / b);
			}
			break;

			case OP_NEGATE:
				vm_stack_push(-vm_stack_pop());
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
