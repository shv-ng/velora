#include "sema_internal.h"

void sema_identifier(struct SemaCtx *ctx, struct AstNode *node, struct Type *hint) {
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
  node->symbol = sym;

  sym->is_used = true;
}
