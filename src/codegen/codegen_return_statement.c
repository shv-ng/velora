#include "codegen_internal.h"
#include <llvm-c/Core.h>


void codegen_return_statement(struct CodegenCtx *ctx, struct AstNode *node) {
  LLVMValueRef value = codegen_expression(ctx, node->as.return_statement.expression);
  LLVMBuildRet(ctx->builder, value);
}
