#include "symbol.h"
#include "../utils/da.h"
#include <stdlib.h>

struct Scope *scope_new(struct Arena *a, struct Scope *parent) {
  struct Scope *s = arena_malloc(a, sizeof(struct Scope));

  s->parent = parent;
  s->hashmap = hashmap_new(a);
  s->arena = a;

  s->count = 0;
  s->capacity = 10;
  s->symbols = arena_malloc(a, sizeof(struct Symbol *) * s->capacity);

  return s;
}

struct Symbol *scope_define(struct Scope *s, const char *name,
                            struct Symbol *symbol) {
  if (scope_lookup(s, name)) {
    return NULL;
  }
  hashmap_set(s->hashmap, name, symbol);

  da_append(s->arena, (void ***)&s->symbols, (void *)symbol, &s->count,
            &s->capacity);

  return symbol;
}

struct Symbol *scope_lookup_current(struct Scope *s, const char *name) {
  return hashmap_get(s->hashmap, name);
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

struct Symbol *symbol_new(struct Arena *a, struct AstNode *decl) {
  struct Symbol *sym = arena_malloc(a, sizeof(struct Symbol));
  sym->decl = decl;

  sym->is_defined = true;
  sym->is_moved = false;
  sym->is_used = false;
  sym->type = decl->resolved_type;

  switch (decl->kind) {
  case AST_FUNCTION_DECLARATION:
    sym->name = decl->as.function.name;
    sym->kind = SYMBOL_FUNC;
    break;

  case AST_VARIABLE_DECLARATION:
    sym->name = decl->as.variable_declaration.name;
    sym->kind = SYMBOL_VAR;
    break;

  default:
    break;
  }

  return sym;
}
