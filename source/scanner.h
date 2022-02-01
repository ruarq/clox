#ifndef CLOX_SCANNER_H
#define CLOX_SCANNER_H

#include <string.h>

#include "common.h"

typedef struct
{
	const char *start;
	const char *current;
	size_t line;
} Scanner;

typedef enum
{
	TOKEN_LPAREN, TOKEN_RPAREN,
	TOKEN_LBRACE, TOKEN_RBRACE,
	TOKEN_DOT, TOKEN_COMMA, TOKEN_SEMICOLON,
	TOKEN_PLUS, TOKEN_MINUS, TOKEN_ASTERISK, TOKEN_SLASH,

	TOKEN_BANG, TOKEN_BANG_EQUAL,
	TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
	TOKEN_GREATER, TOKEN_GREATER_EQUAL,
	TOKEN_LESS, TOKEN_LESS_EQUAL,

	TOKEN_IDENT, TOKEN_STRING, TOKEN_NUMBER,

	TOKEN_SUPER, TOKEN_CLASS, TOKEN_FUN, TOKEN_VAR,
	TOKEN_THIS, TOKEN_RETURN, TOKEN_NIL,
	TOKEN_AND, TOKEN_OR,
	TOKEN_WHILE, TOKEN_FOR, TOKEN_IF, TOKEN_ELSE,
	TOKEN_TRUE, TOKEN_FALSE,
	TOKEN_PRINT,

	TOKEN_ERROR,
	TOKEN_EOF
} TokenType;

typedef struct
{
	TokenType type;
	const char *start;
	size_t size;
	size_t line;
} Token;

void scanner_init(const char *source);

Token scanner_next();
Token scanner_create_token(const TokenType type);
Token scanner_error_token(const char *error);
Token scanner_read_string();
Token scanner_read_number();
Token scanner_read_ident();
TokenType scanner_determine_ident_type();
TokenType scanner_check_keyword(const size_t start, const char *match, const TokenType type);

char scanner_current();
char scanner_advance();
bool scanner_match(const char c);

void scanner_skip_whitespace();
bool scanner_eof();

#endif