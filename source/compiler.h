#ifndef CLOX_COMPILER_H
#define CLOX_COMPILER_H

#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "scanner.h"
#include "chunk.h"

typedef struct
{
	Token previous;
	Token current;

	bool had_error;
	bool panic_mode;
} Parser;

typedef enum
{
	PREC_NONE,
	PREC_ASSIGNMENT,	// =
	PREC_OR,			// or
	PREC_AND,			// and
	PREC_EQUALITY,		// == !=
	PREC_COMPARISON,	// < > <= >=
	PREC_TERM,			// + -
	PREC_FACTOR,		// * /
	PREC_UNARY,			// ! -
	PREC_CALL,			// . ()
	PREC_PRIMARY
} Precedence;

typedef void(*ParseFn)();

typedef struct
{
	ParseFn prefix;
	ParseFn infix;
	Precedence prec;
} ParseRule;

void parse_precedence(const Precedence prec);
ParseRule* get_rule(const TokenType type);

bool compile(const char *source, Chunk *chunk);
void compiler_advance();
void compiler_emit_byte(const uint8_t byte);
void compiler_emit_bytes(const uint8_t byte_a, const uint8_t byte_b);
void compiler_consume(const TokenType type, const char *error);
void compiler_finish();

void compile_expression();
void compile_number();
void compile_grouping();
void compile_unary();
void compile_binary();

Chunk* compiler_current_chunk();

void compiler_error_at(const Token *token, const char *error);
void compiler_error(const char *error);
void compiler_error_at_current(const char *error);

#endif
