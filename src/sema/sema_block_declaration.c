#include "sema_internal.h"
#include "symbol.h"

static void sema_report_unused(struct SemaCtx *ctx) {
  for (size_t i = 0; i < ctx->current_scope->count; i++) {

    struct Symbol *sym = ctx->current_scope->symbols[i];

    if (!sym->is_used && sym->name[0] != '_') {
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

void sema_block_declaration(struct SemaCtx *ctx, struct AstNode *node, struct Type *hint) {
  struct Scope *prev = ctx->current_scope;

  ctx->current_scope = scope_new(ctx->arena, ctx->current_scope);

  node->resolved_type = &type_void;
  for (int i = 0; i < node->as.block.count; i++) {
    sema_node(ctx, node->as.block.statements[i], NULL);
  }

  if (node->as.block.trailing_expression != NULL) {
    sema_node(ctx, node->as.block.trailing_expression, hint);
    node->resolved_type = node->as.block.trailing_expression->resolved_type;
  }

  // report unused
  sema_report_unused(ctx);

  ctx->current_scope = prev;
}

