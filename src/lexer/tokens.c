#include "lexer/tokens.h"

// Token functions

Token Token_init(TokenType type, View lexeme, size_t line) {
   Token token;
   token.type = type;
   token.lexeme = lexeme;
   token.line = line;
   return token;
}

// Tokens functions

Tokens Tokens_init(size_t start) {
   Tokens tokens;
   tokens.start = start;
   tokens.count = 0;
   return tokens;
}
