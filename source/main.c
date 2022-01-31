#include <stdio.h>

#include "common.h"
#include "chunk.h"

int main()
{
	Chunk chunk;
	chunk_init(&chunk);

	chunk_write(&chunk, 'H');
	chunk_write(&chunk, 'e');
	chunk_write(&chunk, 'l');
	chunk_write(&chunk, 'l');
	chunk_write(&chunk, 'o');

	for (size_t i = 0; i < chunk.size; ++i)
	{
		putchar(chunk.code[i]);
	}
	putc('\n', stdout);

	return 0;
}