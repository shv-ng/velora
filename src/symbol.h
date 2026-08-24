#ifndef SYMBOL_H
#define SYMBOL_H

#include "arena.h"
#include "ast.h"
#include "hashmap.h"
#include "types.h"
#include <stdbool.h>

enum SymbolKind {
  SYMBOL_VAR,
  SYMBOL_FUNC,
};

struct Symbol {
  char *name;
  struct AstNode *decl;
  struct Type *type;
  enum SymbolKind kind;
  bool is_defined;
  bool is_used;  // use for unused var, error if false
  bool is_moved; // use after move, error if true
};

struct Scope {
  struct Hashmap *hashmap;
  struct Scope *parent;
  struct Arena *a;
};

struct Symbol *symbol_new(struct Arena *a, struct AstNode *decl);
struct Scope *scope_new(struct Arena *a, struct Scope *parent);
struct Symbol *scope_define(struct Scope *s, const char *name,
                            struct Symbol *sym);
struct Symbol *scope_lookup(struct Scope *s, const char *name);

#endif
