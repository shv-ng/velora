#include "sema_internal.h"

void sema_binary_expression(struct SemaCtx *ctx, struct AstNode *node,
                            struct Type *hint) {

  sema_node(ctx, node->as.binary_expression.left, hint);
  sema_node(ctx, node->as.binary_expression.right, hint);

  if (!type_equal(node->as.binary_expression.left->resolved_type,
                  node->as.binary_expression.right->resolved_type)) {
    emit_error(ctx->err, node->span, ERR_TYPE_MISMATCH,
               type_str(node->as.binary_expression.left->resolved_type),
               type_str(node->as.binary_expression.right->resolved_type));
    return;
  }

  node->resolved_type = node->as.binary_expression.left->resolved_type;
}
