#include "sema_internal.h"

#include <string.h>

struct SemaCtx sema_new(struct ParserCtx *p) {
  struct SemaCtx ctx;

  ctx.error_count = 0;
  ctx.current_return_type = &type_unknown;
  ctx.current_scope = scope_new(p->arena, NULL);
  ctx.file_name = p->lexer->file_name;
  ctx.contents = p->lexer->contents;
  ctx.arena = p->arena;

  return ctx;
}

void sema_node(struct SemaCtx *ctx, struct AstNode *node, struct Type *hint) {
  switch (node->kind) {
  case AST_PROGRAM:
    sema_program(ctx, node);
    break;
  case AST_FUNCTION_DECLARATION:
    sema_function_declaration(ctx, node);
    break;
  case AST_BLOCK_DECLARATION:
    sema_block_declaration(ctx, node, hint);
    break;
  case AST_RETURN_STATEMENT:
    sema_return_statement(ctx, node);
    break;
  case AST_INT_LITERAL:
    node->resolved_type = hint ? hint : &type_unknown;
    break;
  case AST_TYPE_NAMED:
    node->resolved_type = sema_resolve_type_node(node);
    break;
  case AST_BINARY_EXPRESSION:
    sema_binary_expression(ctx, node, hint);
    break;
  case AST_UNARY_EXPRESSION:
    sema_unary_expression(ctx, node, hint);
    break;
  case AST_VARIABLE_DECLARATION:
    sema_variable_declaration(ctx, node);
    break;
  case AST_IDENTIFIER:
    sema_identifier(ctx, node, hint);
    break;
  default:
    break;
  }
}
