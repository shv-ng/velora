
#include "symbol.h"
#include "hashmap.h"
#include <stdlib.h>

struct Scope *scope_new(struct Scope *parent) {
  struct Scope *s = malloc(sizeof(struct Scope));

  s->parent = parent;
  s->hashmap = hashmap_new();

  return s;
}

void scope_free(struct Scope *s) {
  hashmap_free(s->hashmap);
  free(s);
}

struct Symbol *scope_define(struct Scope *s, const char *name,
                            struct Symbol *symbol) {
  if (scope_lookup(s, name)) {
    return NULL;
  }
  hashmap_set(s->hashmap, name, symbol);
  return symbol;
}

struct Symbol *scope_lookup(struct Scope *s, const char *name) {
  for (; s; s = s->parent) {
    struct Symbol *sym = (struct Symbol *)hashmap_get(s->hashmap, name);
    if (sym) {
      return sym;
    }
  }
  return NULL;
}

struct Symbol *symbol_new(struct AstNode *decl) {
  struct Symbol *sym = malloc(sizeof(struct Symbol));
  sym->decl = decl;

  sym->is_defined = true;
  sym->is_moved = false;
  sym->is_used = false;

  switch (decl->kind) {
  case AST_FUNCTION_DECL:
      sym->name = decl->as.function.name;
    break;
  default:
    break;
  }

  return sym;
}
