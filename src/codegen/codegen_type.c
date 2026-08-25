#include "codegen_internal.h"

LLVMTypeRef type_to_llvm(struct CodegenCtx *ctx, struct Type *type) {
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
