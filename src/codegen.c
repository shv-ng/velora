
#include "codegen.h"
#include "ast.h"
#include "error.h"
#include "types.h"
#include <llvm-c/Core.h>
#include <llvm-c/Types.h>
#include <stdio.h>

static LLVMTypeRef type_to_llvm(struct CodegenCtx *ctx, struct Type *type) {
  switch (type->kind) {
  case TYPE_I8:
    return LLVMInt8TypeInContext(ctx->context);
  case TYPE_I32:
    return LLVMInt32TypeInContext(ctx->context);
  case TYPE_VOID:
    return LLVMVoidTypeInContext(ctx->context);
  case TYPE_UNKNOWN:
  case TYPE_FUNC:
    return NULL;
  }
}

void codegen_return(struct CodegenCtx *ctx, struct AstNode *node) {}
void codegen_block(struct CodegenCtx *ctx, struct AstNode *node) {}

void codegen_stmt(struct CodegenCtx *ctx, struct AstNode *node) {
  switch (node->kind) {
  case AST_BLOCK_DECL:
    break;
  case AST_RETURN_STMT:
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
void codegen_func(struct CodegenCtx *ctx, struct AstNode *node) {
  LLVMTypeRef ret_type = type_to_llvm(ctx, node->resolved_type);
  LLVMTypeRef func_type = LLVMFunctionType(ret_type, NULL, 0, 0);

  LLVMValueRef func =
      LLVMAddFunction(ctx->module, node->as.function.name, func_type);
  ctx->current_func = func;

  LLVMBasicBlockRef entry =
      LLVMAppendBasicBlockInContext(ctx->context, func, "entry");
  LLVMPositionBuilderAtEnd(ctx->builder, entry);

  codegen_stmt(ctx, node->as.function.block);
}

static LLVMValueRef codegen_int_literal(struct CodegenCtx *ctx,
                                        struct AstNode *node) {
  LLVMTypeRef t = type_to_llvm(ctx, node->resolved_type);
  return LLVMConstInt(t, (unsigned long long)node->as.int_literal.value, 1);
}

struct CodegenCtx codegen_new() {

  struct CodegenCtx ctx = {0};

  ctx.context = LLVMContextCreate();
  ctx.module = LLVMModuleCreateWithNameInContext("velora", ctx.context);
  ctx.builder = LLVMCreateBuilderInContext(ctx.context);

  ctx.error_count = 0;

  return ctx;
}

void codegen_emit(struct CodegenCtx *ctx, struct AstNode *root) {
  for (int i = 0; i < root->as.program.count; i++) {
    struct AstNode *decl = root->as.program.declaration[i];

    switch (decl->kind) {
    case AST_FUNCTION_DECL:
      codegen_func(ctx, decl);
      break;

    default:
      break;
    }
  }
}
