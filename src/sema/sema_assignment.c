#include "sema.h"
#include "sema_internal.h"
#include "symbol.h"
#include <stdio.h>
#include <sys/types.h>

static struct Type *sema_lvalue_type(struct AstNode *node) {
  switch (node->kind) {
  case AST_IDENTIFIER:
    return node->symbol->type;
  default:
    return &type_unknown;
  }
}

static const char *sema_lvalue_name(struct AstNode *node) {
  switch (node->kind) {
  case AST_IDENTIFIER:
    return node->as.identifer.name;
  default:
    return NULL;
  }
}

void sema_assignment(struct SemaCtx *ctx, struct AstNode *node) {
  const char *name = sema_lvalue_name(node->as.assignment.lvalue);
  if (!name) {
    emit_error(ctx->err, node->span, ERR_INVALID_LVALUE);
  }

  struct Symbol *sym = scope_lookup(ctx->current_scope, name);
  if (!sym) {
    emit_error(ctx->err, node->span, ERR_UNDEFINED_IDENTIFIER, name);
    return;
  }

  sema_node(ctx, node->as.assignment.lvalue, NULL);
  struct Type *lvalue_type = sema_lvalue_type(node->as.assignment.lvalue);

  sema_node(ctx, node->as.assignment.rvalue, lvalue_type);
  struct Type *rvalue_type = node->as.assignment.rvalue->resolved_type;

  if (!type_equal(lvalue_type, rvalue_type)) {
    emit_error(ctx->err, node->span, ERR_TYPE_MISMATCH, lvalue_type,
               rvalue_type);
    return;
  }
  node->resolved_type = lvalue_type;
}
