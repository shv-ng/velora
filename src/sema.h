#ifndef SEMA_H
#define SEMA_H

#include "ast.h"
#include "symbol.h"

struct SemaCtx {
  char *contents;
  char *file_name;

  struct Scope *current_scope;
  struct Type *current_return_type;

  int error_count;
};

struct SemaCtx sema_new(struct Parser *p);
void sema_check(struct SemaCtx *sema, struct AstNode *root);

#endif
