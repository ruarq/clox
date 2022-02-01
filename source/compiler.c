#include "compiler.h"

Parser parser;
Chunk *current_chunk;

// feels kinda dumb. many of the rules aren't even needed. TODO: write something that avoids those stupid empty rules
ParseRule rules[] = {
	[TOKEN_LPAREN]			= { compile_grouping,	NULL,			PREC_NONE	},
	[TOKEN_RPAREN]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_LBRACE]			= { NULL,				NULL,			PREC_NONE	}, 
	[TOKEN_RBRACE]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_COMMA]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_DOT]				= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_MINUS]			= { compile_unary,		compile_binary,	PREC_TERM	},
	[TOKEN_PLUS]			= { NULL,				compile_binary,	PREC_TERM	},
	[TOKEN_SEMICOLON]		= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_SLASH]			= { NULL,				compile_binary,	PREC_FACTOR	},
	[TOKEN_ASTERISK]		= { NULL,				compile_binary,	PREC_FACTOR	},
	[TOKEN_BANG]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_BANG_EQUAL]		= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_EQUAL]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_EQUAL_EQUAL]		= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_GREATER]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_GREATER_EQUAL]	= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_LESS]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_LESS_EQUAL]		= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_IDENT]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_STRING]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_NUMBER]			= { compile_number,		NULL,			PREC_NONE	},
	[TOKEN_AND]				= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_CLASS]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_ELSE]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_FALSE]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_FOR]				= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_FUN]				= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_IF]				= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_NIL]				= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_OR]				= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_PRINT]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_RETURN]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_SUPER]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_THIS]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_TRUE]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_VAR]				= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_WHILE]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_ERROR]			= { NULL,				NULL,			PREC_NONE	},
	[TOKEN_EOF]				= { NULL,				NULL,			PREC_NONE	},
};

void parse_precedence(const Precedence prec)
{
	compiler_advance();

	ParseFn prefix = get_rule(parser.previous.type)->prefix;
	if (!prefix)
	{
		compiler_error("Expect expression");
		return;
	}

	prefix();

	while (prec <= get_rule(parser.current.type)->prec)
	{
		compiler_advance();
		ParseFn infix = get_rule(parser.previous.type)->infix;
		infix();
	}
}

ParseRule* get_rule(const TokenType type)
{
	return &rules[type];
}

bool compile(const char *source, Chunk *chunk)
{
	scanner_init(source);
	current_chunk = chunk;

	parser.had_error = false;
	parser.panic_mode = false;

	compiler_advance();
	compile_expression();
	compiler_consume(TOKEN_EOF, "Expect end of expression");
	compiler_finish();

	return !parser.had_error;
}

void compiler_advance()
{
	parser.previous = parser.current;

	while (true)
	{
		parser.current = scanner_next();

		if (parser.current.type != TOKEN_ERROR)
		{
			break;
		}

		compiler_error_at_current(parser.current.start);
	}
}

void compiler_emit_byte(const uint8_t byte)
{
	chunk_write(compiler_current_chunk(), byte, parser.previous.line);
}

void compiler_emit_bytes(const uint8_t byte_a, const uint8_t byte_b)
{
	compiler_emit_byte(byte_a);
	compiler_emit_byte(byte_b);
}

void compiler_consume(const TokenType type, const char *error)
{
	if (parser.current.type == type)
	{
		compiler_advance();
		return;
	}

	compiler_error_at_current(error);
}

void compiler_finish()
{
	compiler_emit_byte(OP_RETURN);
}

void compile_expression()
{
	parse_precedence(PREC_ASSIGNMENT);
}

void compile_number()
{
	const double value = strtod(parser.previous.start, NULL);
	const size_t constant = chunk_write_constant(compiler_current_chunk(), value_create_number(value));
	if (constant > UINT8_MAX)
	{
		compiler_error("Too many constants in one chunk");
	}
	compiler_emit_bytes(OP_CONSTANT, (uint8_t)(constant));
}

void compile_grouping()
{
	compile_expression();
	compiler_consume(TOKEN_RPAREN, "Expect ')' after expression");
}

void compile_unary()
{
	const TokenType op = parser.previous.type;

	parse_precedence(PREC_UNARY);

	switch (op)
	{
		case TOKEN_MINUS:
			compiler_emit_byte(OP_NEGATE);
			break;

		default:
			break;
	}
}

void compile_binary()
{
	const TokenType op = parser.previous.type;

	const ParseRule *rule = get_rule(op);
	parse_precedence(rule->prec + 1);

	switch (op)
	{
		case TOKEN_PLUS:		compiler_emit_byte(OP_ADD);	break;
		case TOKEN_MINUS:		compiler_emit_byte(OP_SUB);	break;
		case TOKEN_ASTERISK:	compiler_emit_byte(OP_MUL);	break;
		case TOKEN_SLASH:		compiler_emit_byte(OP_DIV);	break;
		default:
			break;
	}
}

Chunk* compiler_current_chunk()
{
	return current_chunk;
}

void compiler_error_at(const Token *token, const char *error)
{
	if (parser.panic_mode)
	{
		return;
	}
	parser.panic_mode = true;

	fprintf(stderr, "[line %ld] Error", token->line);
	
	if (token->type == TOKEN_EOF)
	{
		fprintf(stderr, " at end");
	}
	else if (token->type == TOKEN_ERROR)
	{
		// Nothing.? Why nothing, why even write this???????
	}
	else
	{
		fprintf(stderr, " at '%.*s'", (int)(token->size), token->start);
	}

	fprintf(stderr, ": %s\n", error);
	parser.had_error = true;
}

void compiler_error(const char *error)
{
	compiler_error_at(&parser.previous, error);
}

void compiler_error_at_current(const char *error)
{
	compiler_error_at(&parser.current, error);
}
