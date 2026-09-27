#include "sema_internal.h"
#include <stdbool.h>

void sema_function_declaration(struct SemaCtx *ctx, struct AstNode *node) {
  struct Type *prev_type = ctx->current_return_type;

  sema_node(ctx, node->as.function.return_type, prev_type);

  ctx->current_return_type =
      sema_resolve_type_node(node->as.function.return_type);
  node->resolved_type = ctx->current_return_type;

  sema_node(ctx, node->as.function.block, ctx->current_return_type);

  if (!type_equal(ctx->current_return_type, &type_void)) {

    struct AstNode *block = node->as.function.block;

    bool has_expr = block->as.block.trailing_expression != NULL;
    bool has_return =
        block->as.block.count > 0 &&
        block->as.block.statements[block->as.block.count - 1]->kind ==
            AST_RETURN_STATEMENT;

    if (!has_expr && !has_return) {

      emit_error(ctx->err, node->as.function.block->span, ERR_MISSING_RETURN,
                 node->as.function.name, type_str(ctx->current_return_type));
    }
  }

  ctx->current_return_type = prev_type;
}
