#ifndef HASHMAP_H
#define HASHMAP_H

struct Entry {
  char *key;
  struct Entry *next;
  void *data;
};

struct Hashmap {
  struct Entry **buckets;
  int capacity;
};

struct Hashmap *hashmap_new(void);
void hashmap_free(struct Hashmap *hmap);
void hashmap_set(struct Hashmap *hmap, const char *key, void *value);
void *hashmap_get(struct Hashmap *hmap, const char *key);

#endif
