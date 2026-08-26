#include "sema_internal.h"

void sema_block(struct SemaCtx *sema, struct AstNode *node, struct Type *hint) {
  struct Scope *prev = sema->current_scope;

  sema->current_scope = scope_new(sema->arena, sema->current_scope);

  node->resolved_type = &type_void;
  for (int i = 0; i < node->as.block.count; i++) {
    sema_node(sema, node->as.block.statements[i], NULL);
  }

  if (node->as.block.trailing_expr != NULL) {
    sema_node(sema, node->as.block.trailing_expr, hint);
    node->resolved_type = node->as.block.trailing_expr->resolved_type;
  }

  sema->current_scope = prev;
}

void sema_return_stmt(struct SemaCtx *sema, struct AstNode *node) {

  if (!node->as.return_stmt.expr) {
    struct Error err = {.kind = ERR_SYNTAX,
                        .span = node->span,
                        .as.syntax = {
                            .expected = type_str(sema->current_return_type),
                        }};
    sema->error_count++;
    print_error(err, sema->file_name, sema->contents);
    return;
  }

  sema_node(sema, node->as.return_stmt.expr, sema->current_return_type);
  struct Type *actual = node->as.return_stmt.expr->resolved_type;
  node->resolved_type = actual;

  if (!type_equal(actual, sema->current_return_type)) {
    struct Error err = {
        .kind = ERR_TYPE_MISMATCH,
        .span = node->span,
        .as.type_mismatch =
            {
                .expected = type_str(sema->current_return_type),
                .found = type_str(actual),
                .context = "return statement",
            },
    };
    sema->error_count++;
    print_error(err, sema->file_name, sema->contents);
  }
}
