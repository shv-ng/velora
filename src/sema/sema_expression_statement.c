#include "sema.h"
#include "sema_internal.h"

void sema_expression_statement(struct SemaCtx *ctx, struct AstNode *node) {
  sema_node(ctx, node->as.expression_statement.expression, &type_void);
  node->resolved_type = &type_void;
}
