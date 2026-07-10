#include "hashmap.h"
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

struct Hashmap *hashmap_new(void) {
  struct Hashmap *h = malloc(sizeof(struct Hashmap));

  h->capacity = 1024; // hardcoded for now, which is fine for simplicity
  h->buckets = calloc(1024, sizeof(struct Entry *));

  return h;
};

void hashmap_free(struct Hashmap *hmap) {
  for (int i = 0; i < hmap->capacity; i++) {
    struct Entry *e = hmap->buckets[i];
    while (e) {
      struct Entry *next = e->next;
      free(e->key);
      free(e);
      e = next;
    }
  }
  free(hmap->buckets);
  free(hmap);
}

void hashmap_set(struct Hashmap *hmap, const char *key, void *value) {
  unsigned long idx = hash(key, hmap->capacity);

  struct Entry *e = malloc(sizeof(struct Entry));
  e->key = strdup(key);
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
