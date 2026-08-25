#include "sema_internal.h"

void sema_func(struct SemaCtx *sema, struct AstNode *node) {
  struct Type *prev_type = sema->current_return_type;

  sema_node(sema, node->as.function.return_type, NULL);

  sema->current_return_type = resolve_type_node(node->as.function.return_type);
  node->resolved_type = sema->current_return_type;

  sema_node(sema, node->as.function.block, NULL);

  sema->current_return_type = prev_type;
}
