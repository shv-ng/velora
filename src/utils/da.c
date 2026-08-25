#include "da.h"
#include "arena.h"
#include <stddef.h>

void da_append(struct Arena *a, void ***array, void *value, size_t *count,
               size_t *capacity) {
  if (*count >= *capacity) {
    size_t old_size = *capacity * sizeof(void *);
    *capacity *= 2;
    size_t new_size = *capacity * sizeof(void *);
    *array = arena_realloc(a, *array, old_size, new_size);
  }

  (*array)[(*count)++] = value;
}
