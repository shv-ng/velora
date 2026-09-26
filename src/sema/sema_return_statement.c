#include "sema_internal.h"

void sema_return_statement(struct SemaCtx *ctx, struct AstNode *node) {

  if (!node->as.return_statement.expression) {
    struct Error err = {.kind = ERR_SYNTAX,
                        .span = node->span,
                        .as.syntax = {
                            .expected = type_str(ctx->current_return_type),
                        }};
    ctx->error_count++;
    print_error(err, ctx->file_name, ctx->contents);
    return;
  }

  sema_node(ctx, node->as.return_statement.expression,
            ctx->current_return_type);

  struct Type *actual = node->as.return_statement.expression->resolved_type;

  node->resolved_type = actual;

  if (!type_equal(actual, ctx->current_return_type)) {
    struct Error err = {
        .kind = ERR_TYPE_MISMATCH,
        .span = node->span,
        .as.type_mismatch =
            {
                .expected = type_str(ctx->current_return_type),
                .found = type_str(actual),
                .context = "return statement",
            },
    };
    ctx->error_count++;
    print_error(err, ctx->file_name, ctx->contents);
  }
}
