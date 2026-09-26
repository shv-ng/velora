#include "codegen.h"
#include <llvm-c/Core.h>

struct CodegenCtx codegen_new(struct SemaCtx *sema_ctx) {

  struct CodegenCtx ctx = {.file_name = sema_ctx->file_name,
                           .contents = sema_ctx->contents};

  ctx.context = LLVMContextCreate();
  ctx.module = LLVMModuleCreateWithNameInContext(ctx.file_name, ctx.context);
  ctx.builder = LLVMCreateBuilderInContext(ctx.context);

  ctx.error_count = 0;

  return ctx;
}

void codegen_free(struct CodegenCtx *ctx) {
  LLVMDisposeBuilder(ctx->builder);
  LLVMDisposeModule(ctx->module);
  LLVMContextDispose(ctx->context);
}
