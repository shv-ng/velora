#include "sema_internal.h"

void sema_unary_expression(struct SemaCtx *ctx, struct AstNode *node,
                           struct Type *hint) {
  sema_node(ctx, node->as.unary_expression.expression, hint);

  node->resolved_type = node->as.unary_expression.expression->resolved_type;
}
