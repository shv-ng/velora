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

struct Hashmap *hmap_new();
void hmap_free(struct Hashmap *hmap);
void hmap_set(struct Hashmap *hmap, const char *key, void *value);
void *hmap_get(struct Hashmap *hmap, const char *key);

#endif
