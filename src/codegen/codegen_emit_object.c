#include "codegen_internal.h"
#include <llvm-c/Core.h>

// emit object file to path
bool codegen_emit_object(struct CodegenCtx *ctx, LLVMTargetMachineRef machine,
                         const char *path) {

  char *emit_err = NULL;
  if (LLVMTargetMachineEmitToFile(machine, ctx->module, (char *)path,
                                  LLVMObjectFile, &emit_err)) {
    ctx->error_count += 1;
    struct Error error = {.kind = ERR_CODEGEN,
                          .as.codegen =
                              (struct ErrCodegen){.message = emit_err}};

    print_error(error, ctx->file_name, ctx->contents);
    return false;
  }

  LLVMDisposeMessage(emit_err);
  return true;
}
