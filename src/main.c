#include "util/fileio.h"
#include "util/string.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
   if (argc < 2) {
      printf("Expected at least 2 arguments, got %d instead.\n", argc);
      exit(EXIT_FAILURE);
   }

   String input = String_copyCstringNew(argv[1]);
   String code = readFileIfGiven(&input);
   printf("'%s' - %lu\n", code.base, code.size);

   String_free(&code);
   return 0;
}
