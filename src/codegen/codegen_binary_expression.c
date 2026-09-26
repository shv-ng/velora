#include "codegen_internal.h"
#include <llvm-c/Core.h>
#include <llvm-c/Types.h>

LLVMValueRef codegen_binary_expression(struct CodegenCtx *ctx,
                                       struct AstNode *node) {

  LLVMValueRef left_val =
      codegen_expression(ctx, node->as.binary_expression.left);

  LLVMValueRef right_val =
      codegen_expression(ctx, node->as.binary_expression.right);

  switch (node->as.binary_expression.op) {
  case OP_ADD:
    return LLVMBuildAdd(ctx->builder, left_val, right_val, "add");
  case OP_SUB:
    return LLVMBuildSub(ctx->builder, left_val, right_val, "sub");
  case OP_MULIPLY:
    return LLVMBuildMul(ctx->builder, left_val, right_val, "mul");
  case OP_DIVIDE:
    return LLVMBuildSDiv(ctx->builder, left_val, right_val, "div");
  case OP_MODULO:
    return LLVMBuildSRem(ctx->builder, left_val, right_val, "mod");
  case OP_LEFT_SHIFT:
    return LLVMBuildShl(ctx->builder, left_val, right_val, "left_shift");
  case OP_RIGHT_SHIFT:
    return LLVMBuildLShr(ctx->builder, left_val, right_val, "right_shift");
  case OP_BITWISE_XOR:
    return LLVMBuildXor(ctx->builder, left_val, right_val, "xor");
  case OP_BITWISE_OR:
    return LLVMBuildOr(ctx->builder, left_val, right_val, "or");
  case OP_BITWISE_AND:
    return LLVMBuildAnd(ctx->builder, left_val, right_val, "and");
  default:
    return NULL;
  }
}
