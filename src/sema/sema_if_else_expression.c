#include "sema.h"
#include "sema_internal.h"

void sema_if_else_expression(struct SemaCtx *ctx, struct AstNode *node,
                             struct Type *hint) {
  hint = hint ? hint : &type_void;

  sema_node(ctx, node->as.if_else_expression.condition, &type_bool);

  struct AstNode *if_block = node->as.if_else_expression.if_block;
  sema_node(ctx, if_block, hint);

  if (!type_equal(hint, if_block->resolved_type)) {
    emit_error(ctx->err, if_block->span, ERR_TYPE_MISMATCH, type_str(hint),
               if_block->resolved_type);
  }

  struct AstNode *else_block = node->as.if_else_expression.else_block;
  if (else_block) {
    sema_node(ctx, else_block, hint);
    if (!type_equal(hint, else_block->resolved_type)) {
      emit_error(ctx->err, if_block->span, ERR_TYPE_MISMATCH, type_str(hint),
                 else_block->resolved_type);
    }
  }

  node->resolved_type = hint;
}
