#include <stdint.h>
#include<stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define KiB(n) ((uint64_t)(n)) << 10)
#define MiB(n) ((uint64_t)(n)) << 20)
#define GiB(n) ((uint64_t)(n)) << 30)

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define ALIGN_UP_POW2(n, p) (((uint64_t)(n) + ((uint64_t)(p) - 1)) & (~((uint64_t)(p) - 1)))

#define ARENA_BASE_POS (sizeof(mem_arena))
#define ARENA_ALIGN (sizeof(void*))
typedef struct {
    uint64_t capacity;
    uint64_t pos;
} mem_arena;

mem_arena* arena_create(uint64_t capacity);
void arena_destroy(mem_arena* arena);
void* arena_push(mem_arena* arena, uint64_t size, bool non_zero);
void arena_pop(mem_arena* arena, uint64_t size);
void arena_pop_to(mem_arena* arena, uint64_t pos);
void arena_pop_clear(mem_arena* arena);

void main(void) {
    
    return 0;
}


mem_arena* arena_create(uint64_t capacity) {
    mem_arena* arena = (mem_arena*)malloc(capacity);

    arena->capacity = capacity;
    arena->pos = ARENA_BASE_POS;

    return arena;
}

void arena_destroy(mem_arena* arena) {
    free(arena);
}

void* arena_push(mem_arena* arena, uint64_t size, bool non_zero) {
    uint64_t pos_aligned = ALIGN_UP_POW2(arena->pos, ARENA_ALIGN);
    uint64_t new_pos = pos_aligned + size;

    if (new_pos > arena->capacity) {
        printf("New position is greater than the arena capacity. Aborting...");
        abort();
    }

    arena->pos = new_pos;

    uint8_t out = (uint8_t*)arena + pos_aligned;
    
    if (!non_zero) {
        memset(out, 0 ,size);
    }

    return out;
}

void arena_pop(mem_arena* arena, uint64_t size) {
    size = MIN(size, arena->pos - ARENA_BASE_POS);
    arena->pos -= size;   
}

void arena_pop_to(mem_arena* arena, uint64_t pos) {
    uint64_t size = pos < arena->pos ? arena->pos - pos : 0;
    arena_pop(arena, size);
}

void arena_pop_clear(mem_arena* arena) {
    arena_pop_to(arena, ARENA_BASE_POS);
}