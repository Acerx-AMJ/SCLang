#include "util/string.h"
#include <stdio.h>

int main() {
   String a = String_init(5);

   String_append(&a, 'H');
   String_append(&a, 'e');
   String_append(&a, 'l');
   String_append(&a, 'l');
   String_append(&a, 'o');

   printf("'%s' - s: %lu c: %lu\n", a.base, a.size, a.capacity);

   String_append(&a, 'J');
   String_append(&a, 'a');
   String_append(&a, 'm');
   String_append(&a, 'e');
   String_append(&a, 's');

   printf("'%s' - s: %lu c: %lu\n", a.base, a.size, a.capacity);

   String_concatCstring(&a, ", How are you doing?");

   printf("'%s' - s: %lu c: %lu\n", a.base, a.size, a.capacity);

   String_free(&a);

   /*
   String a = String_copyCstringNew("String1");
   String b = String_copyCstringNew(" + ");
   String c = String_copyCstringNew("String2");

   String_concat(&a, &b);
   String_concat(&a, &c);

   printf("'%s' - s: %lu c: %lu\n", a.base, a.size, a.capacity);
   printf("'%s' - s: %lu c: %lu\n", b.base, b.size, b.capacity);
   printf("'%s' - s: %lu c: %lu\n", c.base, c.size, c.capacity);

   String_free(&a);
   String_free(&b);
   String_free(&c);
   return 0;*/
}
