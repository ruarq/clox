#include "compiler.h"

void compile(const char *source)
{
	scanner_init(source);
	size_t line = -1;
	while (true)
	{
		Token token = scanner_next();
		if (token.line != line)
		{
			printf("%4ld ", line);
			line = token.line;
		}
		else
		{
			printf("   | ");
		}

		printf("%2d '%.*s'\n", token.type, (int)(token.size), token.start);

		if (token.type == TOKEN_EOF)
		{
			break;
		}
	}
}