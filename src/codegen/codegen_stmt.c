#include "codegen_internal.h"
#include <llvm-c/Core.h>

void codegen_return(struct CodegenCtx *ctx, struct AstNode *node) {
  LLVMValueRef value = codegen_expr(ctx, node->as.return_stmt.expr);
  LLVMBuildRet(ctx->builder, value);
}

void codegen_block(struct CodegenCtx *ctx, struct AstNode *node) {
  for (int i = 0; i < node->as.block.count; i++) {
    codegen_node(ctx, node->as.block.statements[i]);
  }

  if (node->as.block.trailing_expr) {
    if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ctx->builder))) {
      LLVMValueRef value = codegen_expr(ctx, node->as.block.trailing_expr);
      LLVMBuildRet(ctx->builder, value);
    }
  } else {
    if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ctx->builder))) {
      LLVMBuildRetVoid(ctx->builder);
    }
  }
}

