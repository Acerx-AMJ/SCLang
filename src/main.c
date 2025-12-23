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

   printf("'%s' - %lu\n", code.base, code.size);

   Arena_free(&arena);
   return 0;
}
