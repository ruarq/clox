#include <stdio.h>

#include "common.h"
#include "vm.h"
#include "chunk.h"
#include "debug.h"

void repl()
{
	char line[1024];
	while (true)
	{
		printf("> ");
		if (!fgets(line, sizeof(line), stdin))
		{
			printf("\n");
			break;
		}

		vm_interpret(line);
	}
}

char *read_file(const char *filename)
{
	FILE *file = fopen(filename, "rb");
	if (!file)
	{
		fprintf(stderr, "Couldn't open file '%s'\n", filename);
		exit(74);
	}

	fseek(file, 0, SEEK_END);
	const size_t file_size = ftell(file);
	rewind(file);

	char *buf = calloc(file_size + 1, sizeof(char));
	fread(buf, sizeof(char), file_size, file);

	fclose(file);
	return buf;
}

void run_file(const char *filename)
{
	char *source = read_file(filename);
	InterpretResult result = vm_interpret(source);
	
	free(source);

	switch (result)
	{
		case INTERPRET_COMPILE_ERROR:
			exit(65);
		
		case INTERPRET_RUNTIME_ERROR:
			exit(70);
	}
}

int main(int argc, char **argv)
{
	switch (argc)
	{
		case 1:
			repl();
			break;

		case 2:
			run_file(argv[1]);
			break;

		default:
			printf("usage: %s [file]\n", argv[0]);
			return 1;
	}
	
	return 0;
}