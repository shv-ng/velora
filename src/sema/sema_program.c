#include "sema_internal.h"

void sema_program(struct SemaCtx *ctx, struct AstNode *root) {
  // collection of global decl
  for (int i = 0; i < root->as.program.count; i++) {
    struct AstNode *decl = root->as.program.declaration[i];

    struct Symbol *sym = symbol_new(ctx->arena, decl);

    switch (decl->kind) {
    case AST_FUNCTION_DECLARATION: {
      struct Symbol *exiting =
          scope_define(ctx->current_scope, decl->as.function.name, sym);

      if (!exiting) {
        emit_error(ctx->err, decl->span, ERR_REDECLARATION,
                   decl->as.function.name);
      }

      decl->symbol = sym;
      break;
    }
    default:
      break;
    }
  }

  // recursive semantic analysis+type checking
  for (int i = 0; i < root->as.program.count; i++) {
    sema_node(ctx, root->as.program.declaration[i], NULL);
  }

  root->resolved_type = &type_void;

  if (ctx->err->count == 0)
    sema_check_type_resolve(ctx, root);
}
