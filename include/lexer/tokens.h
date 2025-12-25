#ifndef LEXER_TOKENS_H
#define LEXER_TOKENS_H

#include "util/view.h"

// Token type

typedef enum {
   EOF, KEYWORD, IDENTIFIER, NUMBER, CHARACTER, STRING,
   INCREMENT, DECREMENT, ASSIGN,
   PLUS_EQUALS, MINUS_EQUALS, MULTIPLY_EQUALS, DIVIDE_EQUALS, REMAINDER_EQUALS, EXPONENTIATE_EQUALS,
   PLUS, MINUS, MULTIPLY, DIVIDE, REMAINDER, EXPONENTIATE,
   LOGICAL_AND, LOGICAL_OR, LOGICAL_NOT, DIVISIBLE, BINARY_CONDITION, QUESTION, COLON, EQUALS, REALLY_EQUALS, NOT_EQUALS, REALLY_NOT_EQUALS, GREATER, GREATER_EQUAL, SMALLER, SMALLER_EQUAL,
   ARROW, THICK_ARROW, LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET, COMMA, DOT_DOT, DOT, SEMICOLON
} TokenType;

#define MAX_OPERATOR_SIZE 3

[[maybe_unused]] static const char *TokenTypeStrings[] = {
   "EOF", "Keyword", "Identifier", "Number", "Character", "String",
   "++", "--", "=",
   "+=", "-=", "*=", "/=", "%=", "**=",
   "+", "-", "*", "/", "%", "**",
   "&&", "||", "!", "%%", "??", "?", ":", "==", "===", "!=", "!==", ">", ">=", "<", "<=",
   "->", "=>", "(", ")", "{", "}", "[", "]", ",", "..", ".", ";"
};

// Token struct

typedef struct {
   TokenType type;
   View lexeme;
   size_t line;
} Token;

Token Token_init(TokenType type, View lexeme, size_t line);

// Tokens structs

typedef struct {
   Token *start;
   size_t count;
} Tokens;

Tokens Tokens_init(size_t start);

#endif
