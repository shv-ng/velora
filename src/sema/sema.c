#include "sema_internal.h"

#include <string.h>

struct SemaCtx sema_new(struct Parser *p) {
  struct SemaCtx ctx;

  ctx.error_count = 0;
  ctx.current_return_type = &type_unknown;
  ctx.current_scope = scope_new(p->arena, NULL);
  ctx.file_name = p->lexer->file_name;
  ctx.contents = p->lexer->contents;
  ctx.arena = p->arena;

  return ctx;
}

void sema_check(struct SemaCtx *ctx, struct AstNode *root) {
  // collection of global decl
  for (int i = 0; i < root->as.program.count; i++) {
    struct AstNode *decl = root->as.program.declaration[i];
    struct Symbol *sym = symbol_new(ctx->arena, decl);
    switch (decl->kind) {
    case AST_FUNCTION_DECL:
      scope_define(ctx->current_scope, decl->as.function.name, sym);
      break;
    default:
      break;
    }
  }

  // recursive semantic analysis+type checking
  for (int i = 0; i < root->as.program.count; i++) {
    sema_node(ctx, root->as.program.declaration[i], NULL);
  }
}

void sema_node(struct SemaCtx *ctx, struct AstNode *node, struct Type *hint) {
  switch (node->kind) {
  case AST_FUNCTION_DECL:
    sema_func(ctx, node);
    break;
  case AST_BLOCK_DECL:
    sema_block(ctx, node, hint);
    break;
  case AST_RETURN_STMT:
    sema_return_stmt(ctx, node);
    break;
  case AST_INT_LITERAL:
    node->resolved_type = hint ? hint : &type_unknown;
    break;
  case AST_TYPE_NAMED:
    node->resolved_type = resolve_type_node(node);
    break;
  case AST_BINARY_EXPR:
    sema_binary_expr(ctx, node, hint);
    break;
  case AST_UNARY_EXPR:
    sema_unary_expr(ctx, node, hint);
    break;
  case AST_VAR_DECL:
    sema_var_decl(ctx, node);
    break;
  case AST_IDENTIFIER:
    sema_identifier(ctx, node, hint);
    break;
  default:
    break;
  }
}
