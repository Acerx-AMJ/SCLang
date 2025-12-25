#ifndef LEXER_LEXER_HPP
#define LEXER_LEXER_HPP

#include "lexer/tokens.h"

// Lexer

typedef struct {
   View *code;
   Arena *arena;

   Arena viewArena;
   Tokens tokens;

   size_t index;
   size_t line;
} Lexer;

// Initializate

Lexer Lexer_init(Arena *arena, View *code);

// Helper functions

char Lexer_current(Lexer *lexer);
char Lexer_peek(Lexer *lexer);
char Lexer_advance(Lexer *lexer);
char Lexer_getEscapeCode(Lexer *lexer, char character);

// Lex functions

Tokens Lexer_lex(Lexer *lexer);
void Lexer_alloc(Lexer *lexer, TokenType type, View lexeme);

// Free

void Lexer_free(Lexer *lexer);

#endif
