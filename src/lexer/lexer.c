#include "lexer/lexer.h"
#include <stdalign.h>
#include <stdio.h>
#include <stdlib.h>

// Initialize

Lexer Lexer_init(Arena *arena, View *code) {
   Lexer lexer;
   lexer.code = code;
   lexer.arena = arena;

   lexer.viewArena = Arena_init(1024 * 1024);
   lexer.tokens = Tokens_init((size_t)arena->base + arena->used);

   lexer.index = 0;
   lexer.line = 0;
   return lexer;
}

// Helper functions

char Lexer_current(Lexer *lexer) {
   if (lexer->index >= lexer->code->size) {
      return 0;
   }
   return lexer->code->base[lexer->index];
}

char Lexer_peek(Lexer *lexer) {
   if (lexer->index + 1 >= lexer->code->size) {
      return 0;
   }
   return lexer->code->base[lexer->index + 1];
}

char Lexer_advance(Lexer *lexer) {
   lexer->index += 1;
   return Lexer_current(lexer);
}

char Lexer_getEscapeCode(char character) {
   if (character == 'a') {
      return '\a';
   } else if (character == 'b') {
      return '\b';
   } else if (character == 't') {
      return '\t';
   } else if (character == 'n') {
      return '\n';
   } else if (character == 'v') {
      return '\v';
   } else if (character == 'f') {
      return '\f';
   } else if (character == 'r') {
      return '\r';
   } else if (character == 'e') {
      return '\e';
   } else if (character == '\\') {
      return '\\';
   } else if (character == '\'') {
      return '\'';
   } else if (character == '"') {
      return '"';
   }

   printf("Unknown escape code '\\%c'.\n", character);
   exit(EXIT_FAILURE);
}

// Lex functions

Tokens Lexer_lex(Lexer *lexer) {
   View lexeme1 = View_copyCstringNew(&lexer->viewArena, "Plus lala ");
   Lexer_alloc(lexer, PLUS, lexeme1);

   lexer->line = 4;

   View lexeme2 = View_copyCstringNew(&lexer->viewArena, "Some keyword");
   Lexer_alloc(lexer, KEYWORD, lexeme2);
   return lexer->tokens;
}

void Lexer_alloc(Lexer *lexer, TokenType type, View lexeme) {
   Arena_allocAligned(lexer->arena, sizeof(Token), alignof(Token));

   Token *tokens = (Token*)lexer->tokens.start;
   tokens[lexer->tokens.count].type = type;
   tokens[lexer->tokens.count].lexeme = lexeme;
   tokens[lexer->tokens.count].line = lexer->line;

   lexer->tokens.count += 1;
}

// Free

void Lexer_free(Lexer *lexer) {
   Arena_free(&lexer->viewArena);
}
