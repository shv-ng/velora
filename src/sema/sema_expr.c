#include "sema_internal.h"

void sema_unary_expr(struct SemaCtx *sema, struct AstNode *node,
                     struct Type *hint) {
  sema_node(sema, node->as.unary_expr.expr, hint);

  node->resolved_type = node->as.unary_expr.expr->resolved_type;
}

void sema_binary_expr(struct SemaCtx *sema, struct AstNode *node,
                      struct Type *hint) {

  sema_node(sema, node->as.binary_expr.left, hint);
  sema_node(sema, node->as.binary_expr.right, hint);

  if (!type_equal(node->as.binary_expr.left->resolved_type,
                  node->as.binary_expr.right->resolved_type)) {
    struct Error err = {
        .kind = ERR_TYPE_MISMATCH,
        .span = node->span,
        .as.type_mismatch = {
            .expected = type_str(node->as.binary_expr.left->resolved_type),
            .found = type_str(node->as.binary_expr.right->resolved_type),
            .context = "binary expression",
        }};
    sema->error_count++;
    print_error(err, sema->file_name, sema->contents);
    return;
  }

  node->resolved_type = node->as.binary_expr.left->resolved_type;
}
