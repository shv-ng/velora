#include "codegen.h"
#include <llvm-c/Core.h>

struct CodegenCtx codegen_new(struct SemaCtx *sema_ctx) {

  struct CodegenCtx ctx = {.err = sema_ctx->err};

  ctx.context = LLVMContextCreate();
  ctx.module =
      LLVMModuleCreateWithNameInContext(ctx.err->file_name, ctx.context);
  ctx.builder = LLVMCreateBuilderInContext(ctx.context);

  ctx.err->count = 0;

  return ctx;
}

void codegen_free(struct CodegenCtx *ctx) {
  LLVMDisposeBuilder(ctx->builder);
  LLVMDisposeModule(ctx->module);
  LLVMContextDispose(ctx->context);
}
