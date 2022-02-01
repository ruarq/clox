#include "scanner.h"

Scanner scanner;

bool is_digit(const char c)
{
	return c >= '0' &&  c <= '9';
}

bool is_alpha(const char c)
{
	return
		(c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		c == '_';
}

void scanner_init(const char *source)
{
	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}

Token scanner_next()
{
	scanner_skip_whitespace();
	
	if (scanner_eof())
	{
		return scanner_create_token(TOKEN_EOF);
	}

	scanner.start = scanner.current;
	const char c = scanner_advance();
	switch (c)
	{
		case '(':	return scanner_create_token(TOKEN_LPAREN);
		case ')':	return scanner_create_token(TOKEN_RPAREN);
		case '{':	return scanner_create_token(TOKEN_LBRACE);
		case '}':	return scanner_create_token(TOKEN_RBRACE);
		case '.':	return scanner_create_token(TOKEN_DOT);
		case ',':	return scanner_create_token(TOKEN_COMMA);
		case ';':	return scanner_create_token(TOKEN_SEMICOLON);
		case '+':	return scanner_create_token(TOKEN_PLUS);
		case '-':	return scanner_create_token(TOKEN_MINUS);
		case '*':	return scanner_create_token(TOKEN_ASTERIK);
		case '/':	return scanner_create_token(TOKEN_SLASH);

		case '!':
			return scanner_create_token(scanner_match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);

		case '=':
			return scanner_create_token(scanner_match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);

		case '<':
			return scanner_create_token(scanner_match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);

		case '>':
			return scanner_create_token(scanner_match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);

		case '"':
			return scanner_read_string();

		case '0'...'9':
			return scanner_read_number();

		case 'a'...'z':
		case 'A'...'Z':
		case '_':
			return scanner_read_ident();

		default:
			return scanner_error_token("Unexpected token");
	}
}

Token scanner_create_token(const TokenType type)
{
	Token token;
	token.type = type;
	token.start = scanner.start;
	token.size = scanner.current - scanner.start;
	token.line = scanner.line;
	return token;
}

Token scanner_error_token(const char *error)
{
	Token token;
	token.type = TOKEN_ERROR;
	token.start = error;
	token.size = strlen(error);
	token.line = scanner.line;
	return token;
}

Token scanner_read_string()
{
	while (scanner_current() != '"' && !scanner_eof())
	{
		if (scanner_current() == '\n')
		{
			++scanner.line;
		}

		scanner_advance();
	}

	// consume "
	scanner_advance();
	return scanner_create_token(TOKEN_STRING);
}

Token scanner_read_number()
{
	while (is_digit(scanner_current()))
	{
		scanner_advance();
	}

	if (scanner_current() == '.' && is_digit(scanner.current[1]))
	{
		scanner_advance();

		while (is_digit(scanner_current()))
		{
			scanner_advance();
		}
	}

	return scanner_create_token(scanner_determine_ident_type());
}

TokenType scanner_determine_ident_type()
{
	const char c = scanner_current();
	switch (c)
	{
		case 'a':	return scanner_check_keyword(1, "nd", TOKEN_AND);
		case 'c':	return scanner_check_keyword(1, "lass", TOKEN_CLASS);
		case 'e':	return scanner_check_keyword(1, "lse", TOKEN_ELSE);
		case 'i':	return scanner_check_keyword(1, "f", TOKEN_IF);
		case 'n':	return scanner_check_keyword(1, "il", TOKEN_NIL);
		case 'o':	return scanner_check_keyword(1, "r", TOKEN_OR);
		case 'p':	return scanner_check_keyword(1, "rint", TOKEN_PRINT);
		case 'r':	return scanner_check_keyword(1, "eturn", TOKEN_RETURN);
		case 's':	return scanner_check_keyword(1, "uper", TOKEN_SUPER);
		case 'v':	return scanner_check_keyword(1, "ar", TOKEN_VAR);
		case 'w':	return scanner_check_keyword(1, "hile", TOKEN_WHILE);

		case 'f':
			if (scanner.current - scanner.start > 1)
			{
				switch (scanner.start[1])
				{
					case 'o':	return scanner_check_keyword(2, "r", TOKEN_FOR);
					case 'a':	return scanner_check_keyword(2, "lse", TOKEN_FALSE);
					case 'u':	return scanner_check_keyword(2, "n", TOKEN_FUN);
				}
			}
			break;

		case 't':
			if (scanner.current - scanner.start > 1)
			{
				switch (scanner.start[1])
				{
					case 'h':	return scanner_check_keyword(2, "is", TOKEN_THIS);
					case 'r':	return scanner_check_keyword(2, "ue", TOKEN_TRUE);
				}
			}
			break;
	}
	
	return TOKEN_IDENT;
}

TokenType scanner_check_keyword(const size_t start, const char *match, const TokenType type)
{
	const size_t length = strlen(match);

	if ((size_t)(scanner.current - scanner.start) == start + length && memcmp(scanner.start + start, match, length) == 0)
	{
		return type;
	}
	else
	{
		return TOKEN_IDENT;
	}
}

Token scanner_read_ident()
{
	while (is_alpha(scanner_current()) && is_digit(scanner_current()))
	{
		scanner_advance();
	}

	return scanner_create_token(TOKEN_IDENT);
}

char scanner_current()
{
	return *scanner.current;
}

char scanner_advance()
{
	return *scanner.current++;
}

bool scanner_match(const char c)
{
	if (scanner_eof())
	{
		return false;
	}

	if (*scanner.current != c)
	{
		return false;
	}

	++scanner.current;
	return true;
}

void scanner_skip_whitespace()
{
	while (true)
	{
		const char c = scanner_current();
		switch (c)
		{
			case ' ':
			case '\t':
			case '\r':
				scanner_advance();
				break;

			case '\n':
				++scanner.line;
				scanner_advance();
				break;

			case '/':
				if (scanner.current[1] == '/')
				{
					while (scanner_current() != '\n')
					{
						scanner_advance();
					}
				}
				else
				{
					return;
				}
				break;

			default:
				return;
		}
	}
}

bool scanner_eof()
{
	return *scanner.current == '\0';
}