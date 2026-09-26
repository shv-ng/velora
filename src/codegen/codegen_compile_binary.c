#include "codegen_internal.h"
#include <llvm-c/Analysis.h>
#include <llvm-c/Core.h>
#include <llvm-c/TargetMachine.h>
#include <stdlib.h>

void codegen_compile_binary(struct CodegenCtx *ctx) {
  // verify llvm mod
  char *err = NULL;
  if (LLVMVerifyModule(ctx->module, LLVMPrintMessageAction, &err)) {
    ctx->error_count += 1;
    struct Error error = {.kind = ERR_CODEGEN,
                          .as.codegen = (struct ErrCodegen){.message = err}};

    print_error(error, ctx->file_name, ctx->contents);
    return;
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
    return;
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

  // emit object file
  char *emit_err = NULL;
  if (LLVMTargetMachineEmitToFile(machine, ctx->module, "/tmp/velora_out.o",
                                  LLVMObjectFile, &emit_err)) {
    ctx->error_count += 1;
    struct Error error = {.kind = ERR_CODEGEN,
                          .as.codegen =
                              (struct ErrCodegen){.message = emit_err}};

    print_error(error, ctx->file_name, ctx->contents);
    return;
  }
  LLVMDisposeMessage(emit_err);
  LLVMDisposeTargetMachine(machine);

  int status = system("clang /tmp/velora_out.o -o /tmp/main -static -O3");

  if (status == -1) {
    ctx->error_count += 1;
    struct Error error = {
        .kind = ERR_CODEGEN,
        .as.codegen = (struct ErrCodegen){.message = "fail to run clang"}};

    print_error(error, ctx->file_name, ctx->contents);
    return;
  }
}
