#include "util/fileio.h"
#include <stdio.h>

String readFileIfGiven(String *input) {
   FILE *file = fopen(input->base, "rw");
   if (!file) {
      return *input;
   }

   fseek(file, 0, SEEK_END);
   long size = ftell(file);
   fseek(file, 0, SEEK_SET);

   String result = String_init(size);
   result.size = size;
   fread(result.base, sizeof(char), size, file);
   fclose(file);

   String_free(input);
   return result;
}
