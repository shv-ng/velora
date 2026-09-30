#include "codegen.h"
#include "codegen_internal.h"
#include <llvm-c/Core.h>
#include <llvm-c/Types.h>

LLVMValueRef codegen_bool_literal(struct CodegenCtx *ctx,
                                  struct AstNode *node) {
  LLVMTypeRef t = type_to_llvm(ctx, node->resolved_type);

  int val = node->as.bool_literal.is_true ? 1 : 0;

  return LLVMConstInt(t, val, 1);
}
