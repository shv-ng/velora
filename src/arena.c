#include "arena.h"
#include "error.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct ArenaChunk *new_chunk(struct ArenaChunk *prev, size_t size) {

  if (size < ARENA_CHUNK_SIZE) {
    size = ARENA_CHUNK_SIZE;
  }

  size_t aligned = (size + ARENA_ALIGN - 1) & ~(ARENA_ALIGN - 1);
  size_t total = sizeof(struct ArenaChunk) + aligned;

  struct ArenaChunk *chunk = malloc(total);
  // can't do anything if no memory
  if (!chunk) {
    struct Error err = {.kind = ERR_MEMORY,
                        .as.memory.message = "out of memory"};
    print_error(err, "", "");
    exit(1);
  }

  chunk->prev_chunk = prev;
  chunk->begin = (char *)(chunk + 1);
  chunk->curr = chunk->begin;
  chunk->end = chunk->begin + size;

  return chunk;
}

struct Arena *arena_new(void) {
  struct ArenaChunk *chunk = new_chunk(NULL, 0); // default ARENA_CHUNK_SIZE

  struct Arena *a = malloc(sizeof(struct Arena));
  // can't do anything if no memory
  if (!a) {
    struct Error err = {.kind = ERR_MEMORY,
                        .as.memory.message = "out of memory"};
    print_error(err, "", "");
    exit(1);
  }
  a->head = chunk;
  return a;
}

void *arena_malloc(struct Arena *a, size_t size) {
  size = (size + ARENA_ALIGN - 1) & ~(ARENA_ALIGN - 1);

  if (a->head->curr + size > a->head->end) {
    a->head = new_chunk(a->head, size);
  }

  void *ptr = a->head->curr;
  a->head->curr += size;
  return ptr;
}

void *arena_calloc(struct Arena *a, size_t count, size_t size) {
  void *ptr = arena_malloc(a, count * size);
  memset(ptr, 0, count * size);
  return ptr;
}

void *arena_realloc(struct Arena *a, void *ptr, size_t old_size,
                    size_t new_size) {

  void *new_ptr = arena_malloc(a, new_size);
  memcpy(new_ptr, ptr, old_size);

  return new_ptr;
}

char *arena_strdup(struct Arena *a, const char *s) {
  size_t len = strlen(s) + 1;

  char *copy = arena_malloc(a, len);
  memcpy(copy, s, len);

  return copy;
}

char *arena_strndup(struct Arena *a, const char *s, size_t size) {
  char *copy = arena_malloc(a, size + 1);

  memcpy(copy, s, size);
  copy[size] = '\0';

  return copy;
}
void arena_free(struct Arena *a) {
  struct ArenaChunk *chunk = a->head;

  while (chunk) {
    struct ArenaChunk *prev = chunk->prev_chunk;
    free(chunk);
    chunk = prev;
  }

  free(a);
}
