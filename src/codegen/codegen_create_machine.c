#include "codegen_internal.h"
#include <llvm-c/Analysis.h>
#include <llvm-c/Core.h>

// verify and create target machine
LLVMTargetMachineRef codegen_create_machine(struct CodegenCtx *ctx) {
  // verify llvm mod
  char *err = NULL;
  if (LLVMVerifyModule(ctx->module, LLVMPrintMessageAction, &err)) {
    ctx->error_count += 1;
    struct Error error = {.kind = ERR_CODEGEN,
                          .as.codegen = (struct ErrCodegen){.message = err}};

    print_error(error, ctx->file_name, ctx->contents);
    return NULL;
  }

  LLVMDisposeMessage(err);

  // get target triple: TODO: will change as per user given
  char *triple = LLVMGetDefaultTargetTriple();

  // initialise targets
  // LLVMInitializeAllTargetInfos();
  // LLVMInitializeAllTargets();
  // LLVMInitializeAllTargetMCs();
  // LLVMInitializeAllAsmParsers();
  // LLVMInitializeAllAsmPrinters();
  LLVMInitializeNativeTarget();
  LLVMInitializeNativeAsmPrinter();
  LLVMInitializeNativeAsmParser();

  // get target from triple
  LLVMTargetRef target;
  char *target_err = NULL;

  if (LLVMGetTargetFromTriple(triple, &target, &target_err)) {
    ctx->error_count += 1;
    struct Error error = {.kind = ERR_CODEGEN,
                          .as.codegen =
                              (struct ErrCodegen){.message = target_err}};

    print_error(error, ctx->file_name, ctx->contents);
    return NULL;
  }

  LLVMDisposeMessage(target_err);

  // create target machine and config module
  LLVMTargetMachineRef machine = LLVMCreateTargetMachine(
      target, triple, "generic", "", LLVMCodeGenLevelDefault, LLVMRelocDefault,
      LLVMCodeModelDefault);

  LLVMTargetDataRef data_layout = LLVMCreateTargetDataLayout(machine);
  LLVMSetModuleDataLayout(ctx->module, data_layout);
  LLVMSetTarget(ctx->module, triple);
  LLVMDisposeTargetData(data_layout);
  LLVMDisposeMessage(triple);

  return machine;
}
