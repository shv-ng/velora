#include "codegen_internal.h"
#include <llvm-c/Core.h>

void codegen_block(struct CodegenCtx *ctx, struct AstNode *node) {
  for (int i = 0; i < node->as.block.count; i++) {
    codegen_node(ctx, node->as.block.statements[i]);
  }

  if (node->as.block.trailing_expression) {
    if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ctx->builder))) {
      LLVMValueRef value =
          codegen_expression(ctx, node->as.block.trailing_expression);
      LLVMBuildRet(ctx->builder, value);
    }
  } else {
    if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ctx->builder))) {
      LLVMBuildRetVoid(ctx->builder);
    }
  }
}
