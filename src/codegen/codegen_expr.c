#include "codegen_internal.h"
#include <llvm-c/Core.h>

LLVMValueRef codegen_int_literal(struct CodegenCtx *ctx, struct AstNode *node) {
  LLVMTypeRef t = type_to_llvm(ctx, node->resolved_type);
  return LLVMConstInt(t, (unsigned long long)node->as.int_literal.value, 1);
}

LLVMValueRef codegen_unary_expr(struct CodegenCtx *ctx, struct AstNode *node) {
  LLVMValueRef val = codegen_expr(ctx, node->as.unary_expr.expr);
  switch (node->as.unary_expr.op) {
  case OP_NEGATIVE:
    return LLVMBuildNeg(ctx->builder, val, "neg");

  case OP_BITWISE_NOT:
    return LLVMBuildNot(ctx->builder, val, "not");

  default:
    return NULL;
  }
}

LLVMValueRef codegen_expr(struct CodegenCtx *ctx, struct AstNode *node) {
  switch (node->kind) {
  case AST_INT_LITERAL:
    return codegen_int_literal(ctx, node);
  case AST_UNARY_EXPR:
    return codegen_unary_expr(ctx, node);

  default:
    ctx->error_count += 1;

    struct Error err = {.span = node->span,
                        .kind = ERR_CODEGEN,
                        .as.codegen = (struct ErrCodegen){
                            .message = "unhandled node kind in expr"}};

    print_error(err, ctx->file_name, ctx->contents);

    break;
  }
  return NULL;
}
