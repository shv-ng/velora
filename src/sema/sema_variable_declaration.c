#include "sema_internal.h"

void sema_variable_declaration(struct SemaCtx *ctx, struct AstNode *node) {
  sema_node(ctx, node->as.variable_declaration.type, NULL);

  node->resolved_type = node->as.variable_declaration.type->resolved_type;

  sema_node(ctx, node->as.variable_declaration.expression, node->resolved_type);

  if (scope_lookup_current(ctx->current_scope, node->as.variable_declaration.name)) {
    ctx->error_count++;
    struct Error err = {.kind = ERR_REDECLARATION,
                        .span = node->span,
                        .as.redeclaration.name = node->as.variable_declaration.name};
    print_error(err, ctx->file_name, ctx->contents);
    return;
  }

  struct Symbol *sym = symbol_new(ctx->arena, node);
  node->symbol = sym;

  scope_define(ctx->current_scope, node->as.variable_declaration.name, sym);

}
