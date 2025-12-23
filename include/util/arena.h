#ifndef UTIL_ARENA_H
#define UTIL_ARENA_H

#include <stddef.h>

typedef struct {
    char *base;
    size_t size;
    size_t used;
} Arena;

// Initialize 

Arena Arena_init(size_t size);

// Allocate

void* Arena_memcpy(Arena *arena, const void *mem, size_t size);
void* Arena_alloc(Arena *arena, size_t size);
void* Arena_allocAligned(Arena *arena, size_t size, size_t alignment);

// Free

void Arena_reset(Arena *arena);
void Arena_free(Arena *arena);

#endif
