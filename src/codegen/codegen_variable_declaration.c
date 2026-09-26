#include "codegen_internal.h"
#include <llvm-c/Core.h>

void codegen_variable_declaration(struct CodegenCtx *ctx,
                                  struct AstNode *node) {

  LLVMBasicBlockRef prev_block = LLVMGetInsertBlock(ctx->builder);

  LLVMBasicBlockRef entry = LLVMGetEntryBasicBlock(ctx->current_func);

  LLVMValueRef first_instr = LLVMGetFirstInstruction(entry);

  if (first_instr) {
    LLVMPositionBuilderBefore(ctx->builder, first_instr);
  } else {
    LLVMPositionBuilderAtEnd(ctx->builder, entry);
  }

  LLVMValueRef slot =
      LLVMBuildAlloca(ctx->builder, type_to_llvm(ctx, node->resolved_type),
                      node->as.variable_declaration.name);
  node->symbol->llvm_slot = slot;

  LLVMPositionBuilderAtEnd(ctx->builder, prev_block);

  LLVMValueRef value =
      codegen_expression(ctx, node->as.variable_declaration.expression);

  LLVMBuildStore(ctx->builder, value, slot);
}
