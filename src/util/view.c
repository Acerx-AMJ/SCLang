#include "util/view.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialize

View View_zeroInit() {
   View view;
   view.base = NULL;
   view.size = 0;
   return view;
}

View View_takeoverNew(Arena *arena, String *string) {
   View view;
   View_takeover(arena, &view, string);
   return view;
}

View View_copyNew(Arena *arena, const String *copyable) {
   View view;
   View_copy(arena, &view, copyable);
   return view;
}

View View_copyCstringNew(Arena *arena, const char *copyable) {
   View view;
   View_copyCstring(arena, &view, copyable);
   return view;
}

void View_takeover(Arena *arena, View *view, String *string) {
   View_copy(arena, view, string);
   String_free(string);
}

void View_copy(Arena *arena, View *view, const String *copyable) {
   view->size = copyable->size;
   view->base = Arena_memcpy(arena, copyable->base, copyable->size + 1);

   if (!view->base) {
      printf("View:View_copy - arena ran out of available memory, size is %lu, but tried to write to %lu.\n", arena->size, arena->used + view->size);
      exit(EXIT_FAILURE);
   }
}

void View_copyCstring(Arena *arena, View *view, const char *copyable) {
   view->size = strlen(copyable);
   view->base = Arena_memcpy(arena, copyable, view->size + 1);

   if (!view->base) {
      printf("View:View_copyCstring - arena ran out of available memory, size is %lu, but tried to write to %lu.\n", arena->size, arena->used + view->size);
      exit(EXIT_FAILURE);
   }
}

// Get functions

char View_at(const View *view, size_t index) {
   if (index >= view->size) {
      printf("View:View_at - index %lu is out of bounds (view size is %lu).\n", index, view->size);
      exit(EXIT_FAILURE);
   }
   return view->base[index];
}

char View_last(const View *view) {
   if (view->size == 0) {
      printf("View:View_last - view is empty.\n");
      exit(EXIT_FAILURE);
   }
   return view->base[view->size - 1];
}

char View_first(const View *view) {
   if (view->size == 0) {
      printf("View:View_first - view is empty.\n");
      exit(EXIT_FAILURE);
   }
   return view->base[0];
}

// Check functions

bool View_empty(const View *view) {
   return view->size == 0;
}

bool View_equals(const View *view1, const View *view2) {
   return memcmp(view1->base, view2->base, view1->size) == 0;
}
