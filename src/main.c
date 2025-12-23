#include "util/arena.h"
#include "util/string.h"
#include "util/view.h"
#include <stdio.h>

int main() {
   String a = String_copyCstringNew("Hello, Wor");
   String_append(&a, 'l');
   String_append(&a, 'd');
   String_append(&a, '!');

   Arena arena = Arena_init(1024 * 1024);
   View v = View_takeoverNew(&arena, &a);

   printf("'%s' %lu\n", v.base, v.size);
   Arena_free(&arena);
   return 0;
}
