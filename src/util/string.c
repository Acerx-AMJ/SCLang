#include "util/string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialize

String String_zeroInit() {
   String string;
   string.size = 0;
   string.capacity = 0;
   string.base = NULL;
   return string;
}

String String_init(size_t capacity) {
   String string;
   string.size = 0;
   string.capacity = capacity;
   string.base = calloc(sizeof(char), capacity + 1);
   string.base[string.capacity] = '\0';
   return string;
}

String String_copyNew(const String *copyable) {
   String string;
   string.base = NULL;
   String_copy(&string, copyable);
   return string;
}

String String_copyCstringNew(const char *copyable) {
   String string;
   string.base = NULL;
   String_copyCstring(&string, copyable);
   return string;
}

void String_copy(String *string, const String *copyable) {
   String_free(string);

   string->size = copyable->size;
   string->capacity = copyable->size;

   string->base = malloc(sizeof(char) * (string->size + 1));
   memcpy(string->base, copyable->base, string->size);
   string->base[string->capacity] = '\0';
}

void String_copyCstring(String *string, const char *copyable) {
   String_free(string);

   string->size = strlen(copyable);
   string->capacity = string->size;

   string->base = malloc(sizeof(char) * (string->size + 1));
   memcpy(string->base, copyable, string->size);
   string->base[string->capacity] = '\0';
}

// Get functions

char String_at(const String *string, size_t index) {
   if (index >= string->size) {
      printf("String:String_at - index %lu is out of bounds (string size is %lu).\n", index, string->size);
      exit(EXIT_FAILURE);
   }
   return string->base[index];
}

char String_last(const String *string) {
   if (string->size == 0) {
      printf("String:String_last - string is empty.\n");
      exit(EXIT_FAILURE);
   }
   return string->base[string->size - 1];
}

char String_first(const String *string) {
   if (string->size == 0) {
      printf("String:String_first - string is empty.\n");
      exit(EXIT_FAILURE);
   }
   return string->base[0];
}

// Check functions

bool String_empty(const String *string) {
   return string->size == 0;
}

bool String_equals(const String *string1, const String *string2) {
   return memcmp(string1->base, string2->base, string1->size) == 0;
}

// String operations

void String_clear(String *string) {
   string->size = 0;
   string->base[string->size] = '\0';
}

void String_concat(String *string1, const String *string2) {
   size_t newSize = string1->size + string2->size;
   String_resize(string1, newSize);
   memcpy(string1->base + string1->size, string2->base, string2->size);
   string1->size = newSize;
   string1->base[string1->size] = '\0';
}

void String_concatCstring(String *string1, const char *string2) {
   size_t newSize = string1->size + strlen(string2);
   String_resize(string1, newSize);
   memcpy(string1->base + string1->size, string2, strlen(string2));
   string1->size = newSize;
   string1->base[string1->size] = '\0';
}

void String_append(String *string, char character) {
   if (string->size + 1 > string->capacity) {
      size_t newCapacity = string->capacity ? string->capacity * 2 : 1;
      String_resize(string, newCapacity);
   }

   string->base[string->size] = character;
   string->size += 1;
   string->base[string->size] = '\0';
}

// Resize functions

void String_resize(String *string, size_t capacity) {
   if (string->capacity < capacity) {
      char *newBase = realloc(string->base, capacity + 1);
      if (!newBase) {
         printf("String:String_resize - failed to reallocate memory.");
         exit(EXIT_FAILURE);
      }

      string->base = newBase;
      string->capacity = capacity;
      string->base[string->capacity] = '\0';
   }
}

void String_resizeToFit(String *string) {
   if (string->size < string->capacity) {
      char *newBase = realloc(string->base, string->size + 1);
      if (!newBase) {
         printf("String:String_resizeToFit - failed to reallocate memory.");
         exit(EXIT_FAILURE);
      }

      string->base = newBase;
      string->capacity = string->size;
      string->base[string->capacity] = '\0';
   }
}

// Free

void String_free(String *string) {
   if (string->base) {
      free(string->base);
      string->base = NULL;
   }
   string->capacity = 0;
   string->size = 0;
}
