#include "sema_internal.h"

void sema_binary_expression(struct SemaCtx *ctx, struct AstNode *node,
                      struct Type *hint) {

  sema_node(ctx, node->as.binary_expression.left, hint);
  sema_node(ctx, node->as.binary_expression.right, hint);

  if (!type_equal(node->as.binary_expression.left->resolved_type,
                  node->as.binary_expression.right->resolved_type)) {
    struct Error err = {
        .kind = ERR_TYPE_MISMATCH,
        .span = node->span,
        .as.type_mismatch = {
            .expected = type_str(node->as.binary_expression.left->resolved_type),
            .found = type_str(node->as.binary_expression.right->resolved_type),
            .context = "binary expression",
        }};
    ctx->error_count++;
    print_error(err, ctx->file_name, ctx->contents);
    return;
  }

  node->resolved_type = node->as.binary_expression.left->resolved_type;
}
