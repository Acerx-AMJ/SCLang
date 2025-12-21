#ifndef UTIL_STRING_H
#define UTIL_STRING_H

#include "util/bool.h"
#include <stddef.h>

typedef struct {
   char *base;
   size_t size;
   size_t capacity;
} String;

// Initialize

String String_zeroInit();
String String_init(size_t capacity);
String String_copyNew(const String *copyable);
String String_copyCstringNew(const char *copyable);

void String_copy(String *string, const String *copyable);
void String_copyCstring(String *string, const char *copyable);

// Get functions

char String_at(const String *string, size_t index);
char String_last(const String *string);
char String_first(const String *string);

// Check functions

bool String_empty(const String *string);
bool String_equals(const String *string1, const String *string2);

// String operations

void String_clear(String *string);
void String_concat(String *string1, const String *string2);
void String_concatCstring(String *string1, const char *string2);
void String_append(String *string, char character);

// Resize functions

void String_resize(String *string, size_t capacity);
void String_resizeToFit(String *string);

// Free

void String_free(String *string);

#endif
