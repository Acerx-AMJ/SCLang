#include "lexer/lexer.h"
#include <ctype.h>
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
   switch (character) {
   case 'a':  return '\a';
   case 'b':  return '\b';
   case 't':  return '\t';
   case 'n':  return '\n';
   case 'v':  return '\v';
   case 'f':  return '\f';
   case 'r':  return '\r';
   case 'e':  return '\e';
   case '\\': return '\\';
   case '\'': return '\'';
   case '"':  return '"';
   default:
      printf("Unknown escape code '\\%c'.\n", character);
      exit(EXIT_FAILURE);
   }
}

// Lex functions

Tokens Lexer_lex(Lexer *lexer) {
   for (char ch = Lexer_current(lexer); lexer->index < lexer->code->size; ch = Lexer_advance(lexer)) {
      if (isspace(ch)) {
         lexer->line += (ch == '\n');
         continue;
      }

      // Handle comments
      if (ch == '/' && Lexer_peek(lexer) == '/') {
         for (; lexer->index < lexer->code->size && ch != '\n'; ch = Lexer_advance(lexer));
         lexer->line += 1;
      } else if (ch == '/' && Lexer_peek(lexer) == '*') {
         size_t originalLine = lexer->line;
         for (; lexer->index < lexer->code->size && (ch != '*' || Lexer_peek(lexer) != '/'); ch = Lexer_advance(lexer)) {
            lexer->line += (ch == '\n');
         }

         Lexer_advance(lexer);
         if (lexer->index >= lexer->code->size) {
            printf("Unterminated block comment at line %lu.\n", originalLine);
            exit(EXIT_FAILURE);
         }
      }
   }
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
