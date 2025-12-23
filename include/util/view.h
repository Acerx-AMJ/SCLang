#ifndef UTIL_VIEW_H
#define UTIL_VIEW_H

#include "util/arena.h"
#include "util/string.h"

typedef struct {
   char *base;
   size_t size;
} View;

// Initialize

View View_zeroInit();
View View_takeoverNew(Arena *arena, String *string);
View View_copyNew(Arena *arena, const String *copyable);
View View_copyCstringNew(Arena *arena, const char *copyable);

void View_takeover(Arena *arena, View *view, String *string);
void View_copy(Arena *arena, View *view, const String *copyable);
void View_copyCstring(Arena *arena, View *view, const char *copyable);

// Get functions

char View_at(const View *view, size_t index);
char View_last(const View *view);
char View_first(const View *view);

// Check functions

bool View_empty(const View *view);
bool View_equals(const View *view1, const View *view2);

#endif
