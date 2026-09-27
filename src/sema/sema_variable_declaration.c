#include "sema_internal.h"

void sema_variable_declaration(struct SemaCtx *ctx, struct AstNode *node) {
  sema_node(ctx, node->as.variable_declaration.type, NULL);

  node->resolved_type = node->as.variable_declaration.type->resolved_type;

  sema_node(ctx, node->as.variable_declaration.expression, node->resolved_type);

  if (scope_lookup_current(ctx->current_scope,
                           node->as.variable_declaration.name)) {
    emit_error(ctx->err, node->span, ERR_REDECLARATION,
               node->as.variable_declaration.name);
    return;
  }

  struct Symbol *sym = symbol_new(ctx->arena, node);
  node->symbol = sym;

  scope_define(ctx->current_scope, node->as.variable_declaration.name, sym);
}
