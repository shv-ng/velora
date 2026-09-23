#pragma once

#include "../ast/ast.h"
#include "../types/types.h"
#include "../utils/arena.h"
#include "../utils/hashmap.h"
#include <llvm-c/Types.h>
#include <stddef.h>

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

  LLVMValueRef llvm_slot; // stack slot filled/used by llvm
};

struct Scope {
  struct Hashmap *hashmap;
  struct Scope *parent;
  struct Arena *arena;

  struct Symbol **symbols;
  size_t count, capacity;
};

struct Symbol *symbol_new(struct Arena *a, struct AstNode *decl);
struct Scope *scope_new(struct Arena *a, struct Scope *parent);
struct Symbol *scope_define(struct Scope *s, const char *name,
                            struct Symbol *sym);
struct Symbol *scope_lookup(struct Scope *s, const char *name);
struct Symbol *scope_lookup_current(struct Scope *s, const char *name);
