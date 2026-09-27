#include "codegen_internal.h"
#include <llvm-c/Core.h>

// emit object file to path
bool codegen_emit_object(struct CodegenCtx *ctx, LLVMTargetMachineRef machine,
                         const char *path) {

  char *emit_err = NULL;
  if (LLVMTargetMachineEmitToFile(machine, ctx->module, (char *)path,
                                  LLVMObjectFile, &emit_err)) {
    emit_error(ctx->err, NO_SPAN, ERR_CODEGEN, emit_err);
    return false;
  }

  LLVMDisposeMessage(emit_err);
  return true;
}
