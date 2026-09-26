#include "codegen_internal.h"
#include <llvm-c/Core.h>

LLVMValueRef codegen_identifier(struct CodegenCtx *ctx, struct AstNode *node) {
  LLVMTypeRef type = type_to_llvm(ctx, node->resolved_type);
  return LLVMBuildLoad2(ctx->builder, type, node->symbol->llvm_slot,
                        node->symbol->name);
}
