#include "sema_internal.h"

void sema_bool_literal(struct SemaCtx *ctx, struct AstNode *node,
                       struct Type *hint) {
  if (!type_equal(&type_bool, hint)) {
    emit_error(ctx->err, node->span, ERR_TYPE_MISMATCH, type_str(&type_bool),
               type_str(hint));
  }
  node->resolved_type = &type_bool;
}
