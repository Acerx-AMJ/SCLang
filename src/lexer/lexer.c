#include "lexer/lexer.h"
#include <stdalign.h>

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

}

char Lexer_peek(Lexer *lexer) {

}

char Lexer_advance(Lexer *lexer) {

}

char Lexer_getEscapeCode(Lexer *lexer, char character) {

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
