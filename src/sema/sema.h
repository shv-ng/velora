#pragma once

#include "../ast/ast.h"
#include "../error/error.h"
#include "../parser/parser.h"
#include "../types/types.h"
#include "../utils/arena.h"
#include "symbol.h"

struct SemaCtx {
  struct Arena *arena;

  struct Scope *current_scope;
  struct Type *current_return_type;

  struct ErrorCtx *err;
};

struct SemaCtx sema_new(struct ParserCtx *p);
void sema_node(struct SemaCtx *ctx, struct AstNode *root, struct Type *hint);
