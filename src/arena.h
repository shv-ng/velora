#ifndef ARENA_H
#define ARENA_H
#include <stddef.h>
#include <stdio.h>

#define ARENA_CHUNK_SIZE (4 * 1024 * 1024) // 4 mb
#define ARENA_ALIGN 8

struct ArenaChunk {
  char *begin;
  char *curr;
  char *end;

  struct ArenaChunk *prev_chunk;
};

struct Arena {
  struct ArenaChunk *head;
};

struct Arena *arena_new(void);
void *arena_malloc(struct Arena *a, size_t size);
void *arena_calloc(struct Arena *a, size_t count, size_t size);
void *arena_realloc(struct Arena *a, void *ptr, size_t old_size,
                    size_t new_size);
char *arena_strdup(struct Arena *a, const char *s);
char *arena_strndup(struct Arena *a, const char *s, size_t size);
void arena_free(struct Arena *a);

#endif
