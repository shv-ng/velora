#include "codegen_internal.h"
#include <llvm-c/Core.h>


LLVMValueRef codegen_unary_expression(struct CodegenCtx *ctx, struct AstNode *node) {
  LLVMValueRef val = codegen_expression(ctx, node->as.unary_expression.expression);
  switch (node->as.unary_expression.op) {
  case OP_NEGATIVE:
    return LLVMBuildNeg(ctx->builder, val, "neg");

  case OP_BITWISE_NOT:
    return LLVMBuildNot(ctx->builder, val, "not");

  default:
    return NULL;
  }
}
 
