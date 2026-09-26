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
    ctx->error_count += 1;

    struct Error err = {.span = node->span,
                        .kind = ERR_CODEGEN,
                        .as.codegen.message = "unhandled node kind in expr"};

    print_error(err, ctx->file_name, ctx->contents);
    break;
  }
  return NULL;
}
