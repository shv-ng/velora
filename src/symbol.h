#ifndef SYMBOL_H
#define SYMBOL_H

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
  AstNode *decl;
  struct Type *type;
  enum SymbolKind kind;
  bool is_defined;
};

struct Scope {
  struct Hashmap *hm;
  struct Scope *parent;
};

struct Scope *scope_new(struct Scope *parent);
void scope_free(struct Scope *s);
struct Symbol *scope_define(struct Scope *s, const char *name,
                            struct Symbol *sym);
struct Symbol *scope_lookup(struct Scope *s, const char *name);

#endif
