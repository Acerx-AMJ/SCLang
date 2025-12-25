#include "lexer/lexer.h"
#include "error/error.h"
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
   lexer.line = 1;
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

char Lexer_getEscapeCode(Lexer *lexer, char character) {
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
      Error_raise(lexer->line, "Unknown escape code '\\%c', did you mean '\\\\%c'?.", character, character);
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
         Error_assert(lexer->index < lexer->code->size, originalLine, "Unterminated block comment at line %lu.\n", originalLine);
      
      // Handle numbers
      } else if (isdigit(ch)) {
         String builder = String_init(5);
         bool dot = false;
         bool lastDash = false;

         for (; lexer->index < lexer->code->size; ch = Lexer_advance(lexer)) {
            if (isdigit(ch)) {
               lastDash = false;
               String_append(&builder, ch);
            } else if (ch == '.' && !isdigit(Lexer_peek(lexer))) {
               break;
            } else if (ch == '.') {
               if (dot) {
                  break;
               }
               dot = true;
               String_append(&builder, ch);
            } else if (ch != '_') {
               break;
            }

            if (ch == '_') {
               Error_assert(!lastDash, lexer->line, "Expected number '%s' to not have two or more consecutive '_'.", builder.base);
               lastDash = true;
            }
         }

         Error_assert(!lastDash, lexer->line, "Expected number '%s' to not end with '_'.", builder.base);
         Lexer_alloc(lexer, NUMBER, &builder);
         lexer->index -= 1;
      
      // Handle identifiers and keywords
      } else if (isalpha(ch) || ch == '_') {
         String builder = String_init(8);

         for (; lexer->index < lexer->code->size; ch = Lexer_advance(lexer)) {
            if (!isalnum(ch) && ch != '_') {
               break;
            }
            lexer->line += (ch == '\n');
            String_append(&builder, ch);
         }

         // TODO: handle keywords and keyword operators
         Lexer_alloc(lexer, IDENTIFIER, &builder);
         lexer->index -= 1;

      // Handle strings
      } else if (ch == '"') {
         String builder = String_init(16);
         size_t originalLine = lexer->line;

         for (ch = Lexer_advance(lexer); lexer->index < lexer->code->size && ch != '"'; ch = Lexer_advance(lexer)) {
            if (ch == '\\') {
               ch = Lexer_getEscapeCode(lexer, Lexer_advance(lexer));
            }
            String_append(&builder, ch);
         }
         Error_assert(ch == '"', originalLine, "Unterminated string.");
         Lexer_alloc(lexer, STRING, &builder);

      // Handle characters
      } else if (ch == '\'') {
         String builder = String_init(1);
         String_append(&builder, Lexer_advance(lexer));

         if (builder.base[0] == '\\') {
            builder.base[0] = Lexer_getEscapeCode(lexer, Lexer_advance(lexer));
         }

         char ch = Lexer_advance(lexer);
         Error_assert(ch == '\'', lexer->line, "Expected character to be one character long/unterminted character.");
         Lexer_alloc(lexer, CHARACTER, &builder);

      // Handle operators
      } else {

      }
   }
   return lexer->tokens;
}

void Lexer_alloc(Lexer *lexer, TokenType type, String *builder) {
   Arena_allocAligned(lexer->arena, sizeof(Token), alignof(Token));
   View lexeme = View_takeoverNew(&lexer->viewArena, builder);

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
