#include "codegen_internal.h"
#include <llvm-c/Core.h>
#include <llvm-c/Types.h>

void codegen_assignment(struct CodegenCtx *ctx, struct AstNode *node) {
  LLVMValueRef value = codegen_expression(ctx, node->as.assignment.rvalue);

  LLVMBuildStore(ctx->builder, value, node->symbol->llvm_slot);
}
