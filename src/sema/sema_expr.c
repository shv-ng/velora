#include "sema.h"
#include "sema_internal.h"
#include "symbol.h"

void sema_unary_expr(struct SemaCtx *ctx, struct AstNode *node,
                     struct Type *hint) {
  sema_node(ctx, node->as.unary_expr.expr, hint);

  node->resolved_type = node->as.unary_expr.expr->resolved_type;
}

void sema_binary_expr(struct SemaCtx *ctx, struct AstNode *node,
                      struct Type *hint) {

  sema_node(ctx, node->as.binary_expr.left, hint);
  sema_node(ctx, node->as.binary_expr.right, hint);

  if (!type_equal(node->as.binary_expr.left->resolved_type,
                  node->as.binary_expr.right->resolved_type)) {
    struct Error err = {
        .kind = ERR_TYPE_MISMATCH,
        .span = node->span,
        .as.type_mismatch = {
            .expected = type_str(node->as.binary_expr.left->resolved_type),
            .found = type_str(node->as.binary_expr.right->resolved_type),
            .context = "binary expression",
        }};
    ctx->error_count++;
    print_error(err, ctx->file_name, ctx->contents);
    return;
  }

  node->resolved_type = node->as.binary_expr.left->resolved_type;
}

void sema_identifier(struct SemaCtx *ctx, struct AstNode *node,
                     struct Type *hint) {
  struct Symbol *sym =
      scope_lookup(ctx->current_scope, node->as.identifer.name);

  if (!sym) {
    struct Error err = {
        .kind = ERR_UNDEFINED_IDENTIFIER,
        .span = node->span,
        .as.undefined_identifier = {.name = node->as.identifer.name}};
    ctx->error_count++;
    print_error(err, ctx->file_name, ctx->contents);
    return;
  }

  if (hint && !type_equal(sym->type, hint)) {
    struct Error err = {.kind = ERR_TYPE_MISMATCH,
                        .span = node->span,
                        .as.type_mismatch = {
                            .expected = type_str(hint),
                            .found = type_str(sym->type),
                        }};
    ctx->error_count++;
    print_error(err, ctx->file_name, ctx->contents);
  }

  node->resolved_type = sym->type;
  sym->is_used = true;
}
