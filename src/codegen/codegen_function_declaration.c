#include "codegen_internal.h"
#include <llvm-c/Core.h>

void codegen_function_declaration(struct CodegenCtx *ctx, struct AstNode *node) {
  LLVMTypeRef ret_type = type_to_llvm(ctx, node->resolved_type);
  LLVMTypeRef func_type = LLVMFunctionType(ret_type, NULL, 0, 0);

  LLVMValueRef func =
      LLVMAddFunction(ctx->module, node->as.function.name, func_type);
  ctx->current_func = func;

  LLVMBasicBlockRef entry =
      LLVMAppendBasicBlockInContext(ctx->context, func, "entry");
  LLVMPositionBuilderAtEnd(ctx->builder, entry);

  codegen_node(ctx, node->as.function.block);
}
