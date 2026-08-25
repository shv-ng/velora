#pragma once

#include "../arena.h"
#include "../ast/ast.h"
#include "../parser/parser.h"
#include "../types/types.h"
#include "symbol.h"

struct SemaCtx {
  char *contents;
  char *file_name;

  struct Scope *current_scope;
  struct Type *current_return_type;
  struct Arena *arena;

  int error_count;
};

struct SemaCtx sema_new(struct Parser *p);
void sema_check(struct SemaCtx *sema, struct AstNode *root);
