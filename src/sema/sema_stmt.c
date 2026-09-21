#include "sema_internal.h"
#include "symbol.h"

static void report_unused(struct SemaCtx *ctx) {
  for (size_t i = 0; i < ctx->current_scope->count; i++) {

    struct Symbol *sym = ctx->current_scope->symbols[i];

    if (!sym->is_used) {

      struct Error err = {
          .kind = ERR_UNUSED_IDENTIFIER,
          .span = sym->decl->span,
          .as.unused_identifier.name = sym->name,
      };

      ctx->error_count++;
      print_error(err, ctx->file_name, ctx->contents);
    }
  }
}

void sema_block(struct SemaCtx *ctx, struct AstNode *node, struct Type *hint) {
  struct Scope *prev = ctx->current_scope;

  ctx->current_scope = scope_new(ctx->arena, ctx->current_scope);

  node->resolved_type = &type_void;
  for (int i = 0; i < node->as.block.count; i++) {
    sema_node(ctx, node->as.block.statements[i], NULL);
  }

  if (node->as.block.trailing_expr != NULL) {
    sema_node(ctx, node->as.block.trailing_expr, hint);
    node->resolved_type = node->as.block.trailing_expr->resolved_type;
  }

  // report unused
  report_unused(ctx);

  ctx->current_scope = prev;
}

void sema_return_stmt(struct SemaCtx *ctx, struct AstNode *node) {

  if (!node->as.return_stmt.expr) {
    struct Error err = {.kind = ERR_SYNTAX,
                        .span = node->span,
                        .as.syntax = {
                            .expected = type_str(ctx->current_return_type),
                        }};
    ctx->error_count++;
    print_error(err, ctx->file_name, ctx->contents);
    return;
  }

  sema_node(ctx, node->as.return_stmt.expr, ctx->current_return_type);
  struct Type *actual = node->as.return_stmt.expr->resolved_type;
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
