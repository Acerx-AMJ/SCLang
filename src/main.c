#include "lexer/lexer.h"
#include "util/arena.h"
#include "util/fileio.h"
#include "util/string.h"
#include "util/view.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
   if (argc < 2) {
      printf("Expected at least 2 arguments, got %d instead.\n", argc);
      exit(EXIT_FAILURE);
   }

   String input = String_copyCstringNew(argv[1]);
   String codeString = readFileIfGiven(&input);

   Arena arena = Arena_init(1024 * 1024);
   View code = View_takeoverNew(&arena, &codeString);

   // Lex the code

   Lexer lexer = Lexer_init(&arena, &code);
   Tokens tokens = Lexer_lex(&lexer);

   Token *tokenArray = (Token*)tokens.start;
   for (int i = 0; i < tokens.count; ++i) {
      Token token = tokenArray[i];
      printf("%d: '%s' - '%s' - %lu\n", i, token.lexeme.base, TokenTypeStrings[token.type], token.line);
   }

   Lexer_free(&lexer);
   Arena_free(&arena);
   return 0;
}
