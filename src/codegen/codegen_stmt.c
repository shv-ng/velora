#include "codegen_internal.h"

void codegen_return(struct CodegenCtx *ctx, struct AstNode *node) {
  LLVMValueRef value = codegen_expr(ctx, node->as.return_stmt.expr);
  LLVMBuildRet(ctx->builder, value);
}

void codegen_block(struct CodegenCtx *ctx, struct AstNode *node) {
  for (int i = 0; i < node->as.block.count; i++) {
    codegen_stmt(ctx, node->as.block.statements[i]);
  }
}

void codegen_stmt(struct CodegenCtx *ctx, struct AstNode *node) {
  switch (node->kind) {
  case AST_BLOCK_DECL:
    codegen_block(ctx, node);
    break;
  case AST_RETURN_STMT:
    codegen_return(ctx, node);
    break;
  default:
    ctx->error_count += 1;

    struct Error err = {
        .span = node->span,
        .kind = ERR_CODEGEN,
        .as.codegen = (struct ErrCodegen){.message = "unhandled node in stmt"}};

    print_error(err, ctx->file_name, ctx->contents);

    break;
  }
}
