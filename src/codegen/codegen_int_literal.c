#include "codegen_internal.h"
#include <llvm-c/Core.h>

LLVMValueRef codegen_int_literal(struct CodegenCtx *ctx, struct AstNode *node) {
  LLVMTypeRef t = type_to_llvm(ctx, node->resolved_type);
  return LLVMConstInt(t, (unsigned long long)node->as.int_literal.value, 1);
}
