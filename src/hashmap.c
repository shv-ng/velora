#include "hashmap.h"
#include "arena.h"
#include <stdlib.h>
#include <string.h>

static unsigned long hash(const char *str, int capacity) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }

  return hash & (capacity - 1);
}

struct Hashmap *hashmap_new(struct Arena *a) {
  struct Hashmap *h = arena_malloc(a, sizeof(struct Hashmap));

  h->capacity =
      HASHMAP_CAPICITY; // hardcoded for now, which is fine for simplicity
  h->buckets = arena_calloc(a, HASHMAP_CAPICITY, sizeof(struct Entry *));
  h->arena = a;

  return h;
};

void hashmap_set(struct Hashmap *hmap, const char *key, void *value) {
  unsigned long idx = hash(key, hmap->capacity);

  struct Entry *e = arena_malloc(hmap->arena, sizeof(struct Entry));
  e->key = arena_strdup(hmap->arena, key);
  e->data = value;
  e->next = hmap->buckets[idx];

  hmap->buckets[idx] = e;
}

void *hashmap_get(struct Hashmap *hmap, const char *key) {
  unsigned long idx = hash(key, hmap->capacity);
  struct Entry *e = hmap->buckets[idx];
  for (; e; e = e->next) {
    if (strcmp(key, e->key) == 0) {
      return e->data;
    }
  }
  return NULL;
}
