#include "codegen.h"
#include "codegen_internal.h"
#include <llvm-c/Core.h>
#include <stdio.h>

void codegen_func(struct CodegenCtx *ctx, struct AstNode *node) {
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

void codegen_var_decl(struct CodegenCtx *ctx, struct AstNode *node) {
  LLVMValueRef slot =
      LLVMBuildAlloca(ctx->builder, type_to_llvm(ctx, node->resolved_type),
                      node->as.var_decl.name);
  node->symbol->llvm_slot = slot;

  LLVMValueRef value = codegen_expr(ctx, node->as.var_decl.expr);

  LLVMBuildStore(ctx->builder, value, slot);
}
