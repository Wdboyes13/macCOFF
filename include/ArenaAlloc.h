#pragma once

#include <stdlib.h>
#include <stdio.h>

#pragma pack(push, 1)
typedef struct {
    void* region;
    size_t region_size;
    size_t unused_offset;
} Arena;
#pragma pack(pop)

static inline Arena* ArenaAlloc(size_t size){
    Arena* ret = (Arena*)malloc(sizeof(Arena));
    ret->region = malloc(size);
    ret->region_size = size;
    return ret;
}

static inline void* ArenaGetPtr(Arena* arena, size_t size){
    if (arena->unused_offset + size > arena->region_size) return NULL;
    void* ret = (char*)arena->region + arena->unused_offset;
    arena->unused_offset += size;
    return ret;
}

static inline void FreeArena(Arena** arena){
    if (*arena){
        free((*arena)->region);
        (*arena)->region = NULL;
        (*arena)->region_size = 0;
        (*arena)->unused_offset = 0;
        free(*arena);
        *arena = NULL;
    }
}