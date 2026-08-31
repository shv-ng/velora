#include "codegen_internal.h"
#include <llvm-c/Core.h>

LLVMValueRef codegen_int_literal(struct CodegenCtx *ctx, struct AstNode *node) {
  LLVMTypeRef t = type_to_llvm(ctx, node->resolved_type);
  return LLVMConstInt(t, (unsigned long long)node->as.int_literal.value, 1);
}

LLVMValueRef codegen_binary_expr(struct CodegenCtx *ctx, struct AstNode *node) {
  LLVMValueRef left_val = codegen_expr(ctx, node->as.binary_expr.left);
  LLVMValueRef right_val = codegen_expr(ctx, node->as.binary_expr.right);

  switch (node->as.binary_expr.op) {
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
    return LLVMBuildOr(ctx->builder, left_val, right_val, "and");
  default:
    return NULL;
  }
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

  case AST_BINARY_EXPR:
    return codegen_binary_expr(ctx, node);

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
