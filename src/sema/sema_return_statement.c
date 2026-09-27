#include "sema_internal.h"

void sema_return_statement(struct SemaCtx *ctx, struct AstNode *node) {

  if (!node->as.return_statement.expression) {
    emit_error(ctx->err, node->span, ERR_UNEXPECTED,
               type_str(ctx->current_return_type));
    return;
  }

  sema_node(ctx, node->as.return_statement.expression,
            ctx->current_return_type);

  struct Type *actual = node->as.return_statement.expression->resolved_type;

  node->resolved_type = actual;

  if (!type_equal(actual, ctx->current_return_type)) {
    emit_error(ctx->err, node->span, ERR_TYPE_MISMATCH_CONTEXT,
               type_str(ctx->current_return_type), type_str(actual),
               "return statement");
  }
}
