#ifndef HASHMAP_H
#define HASHMAP_H

#include "arena.h"

#define HASHMAP_CAPICITY 1024
struct Entry {
  char *key;
  struct Entry *next;
  void *data;
};

struct Hashmap {
  struct Entry **buckets;
  struct Arena *arena;
  int capacity;
};

struct Hashmap *hashmap_new(struct Arena *a);
void hashmap_set(struct Hashmap *hmap, const char *key, void *value);
void *hashmap_get(struct Hashmap *hmap, const char *key);

#endif
