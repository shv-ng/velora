#include "sema_internal.h"
#include <stdbool.h>

void sema_func(struct SemaCtx *sema, struct AstNode *node) {
  struct Type *prev_type = sema->current_return_type;

  sema_node(sema, node->as.function.return_type, prev_type);

  sema->current_return_type = resolve_type_node(node->as.function.return_type);
  node->resolved_type = sema->current_return_type;

  sema_node(sema, node->as.function.block, sema->current_return_type);

  if (!type_equal(sema->current_return_type, &type_void)) {
    struct AstNode *block = node->as.function.block;
    bool has_expr = block->as.block.trailing_expr != NULL;
    bool has_return =
        block->as.block.count > 0 &&
        block->as.block.statements[block->as.block.count - 1]->kind ==
            AST_RETURN_STMT;

    if (!has_expr && !has_return) {
      struct Error err = {.kind = ERR_MISSING_RETURN,
                          .span = node->as.function.block->span,
                          .as.missing_return = {
                              .expected = type_str(sema->current_return_type),
                              .fn_name = node->as.function.name,
                          }};
      sema->error_count++;
      print_error(err, sema->file_name, sema->contents);
    }
  }

  sema->current_return_type = prev_type;
}
