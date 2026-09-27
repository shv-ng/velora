#include "sema_internal.h"

void sema_identifier(struct SemaCtx *ctx, struct AstNode *node,
                     struct Type *hint) {
  struct Symbol *sym =
      scope_lookup(ctx->current_scope, node->as.identifer.name);

  if (!sym) {
    emit_error(ctx->err, node->span, ERR_UNDEFINED_IDENTIFIER,
               node->as.identifer.name);
    return;
  }

  if (hint && !type_equal(sym->type, hint)) {

    emit_error(ctx->err, node->span, ERR_TYPE_MISMATCH, type_str(hint),
               type_str(sym->type));
  }

  node->resolved_type = sym->type;
  node->symbol = sym;

  sym->is_used = true;
}
