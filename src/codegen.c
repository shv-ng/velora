
#include "codegen.h"
#include "ast.h"
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
  }
}
