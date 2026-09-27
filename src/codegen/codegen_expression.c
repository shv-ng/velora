#include "codegen_internal.h"

LLVMValueRef codegen_expression(struct CodegenCtx *ctx, struct AstNode *node) {
  switch (node->kind) {
  case AST_INT_LITERAL:
    return codegen_int_literal(ctx, node);

  case AST_UNARY_EXPRESSION:
    return codegen_unary_expression(ctx, node);

  case AST_BINARY_EXPRESSION:
    return codegen_binary_expression(ctx, node);

  case AST_IDENTIFIER:
    return codegen_identifier(ctx, node);

  default:
    emit_error(ctx->err, NO_SPAN, ERR_CODEGEN, "unhandled node kind in expr");
    break;
  }
  return NULL;
}
