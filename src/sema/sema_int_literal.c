#include "sema.h"
#include <errno.h>
#include <stdlib.h>

void sema_int_literal(struct SemaCtx *ctx, struct AstNode *node,
                      struct Type *hint) {
  hint = type_equal(hint, &type_void) ? NULL : hint;

  node->resolved_type = hint ? hint : &type_i32; // default int will be i32

  // conver to ull
  unsigned long long val = strtoull(node->as.int_literal.raw, NULL, 10);
  if (errno == ERANGE) {
    emit_error(ctx->err, node->span, ERR_OVERFLOW, node->as.int_literal.raw,
               type_str(node->resolved_type));
    return;
  }

  node->as.int_literal.value = val;
}
