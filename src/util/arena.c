#include "util/arena.h"
#include <stdlib.h>
#include <string.h>

// Initialize

Arena Arena_init(size_t size) {
   Arena arena;
   arena.base = malloc(size);
   arena.size = size;
   arena.used = 0;
   return arena;
}

// Allocate

void* Arena_memcpy(Arena *arena, const void *mem, size_t size) {
   if (arena->used + size > arena->size) {
      return NULL;
   }

   void *pointer = arena->base + arena->used;
   memcpy(pointer, mem, size);
   arena->used += size;
   return pointer;
}

void* Arena_alloc(Arena *arena, size_t size) {
   if (arena->used + size > arena->size) {
      return NULL;
   }

   void *pointer = arena->base + arena->used;
   arena->used += size;
   return pointer;
}

void* Arena_allocAligned(Arena *arena, size_t size, size_t alignment) {
   size_t current = (size_t)(arena->base + arena->used);
   size_t aligned = (current + (alignment - 1)) & ~(alignment - 1);
   size_t offset = aligned - (size_t)arena->base;

   if (offset + size > arena->size) {
      return NULL;
   }

   arena->used = offset + size;
   return (void*)aligned;
}

// Free

void Arena_reset(Arena *arena) {
   arena->used = 0;
}

void Arena_free(Arena *arena) {
   if (arena->base) {
      free(arena->base);
      arena->base = NULL;
      arena->size = 0;
      arena->used = 0;
   }
}
