#include "codegen_internal.h"
#include <asm-generic/errno-base.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/Core.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/Types.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void codegen_compile_binary(struct CodegenCtx *ctx) {
  LLVMTargetMachineRef machine = codegen_create_machine(ctx);
  if (!machine) {
    return;
  }

  bool ok = codegen_emit_object(ctx, machine, "/tmp/velora_out.o");

  LLVMDisposeTargetMachine(machine);

  if (!ok) {
    return;
  }

  codegen_link_binary(ctx, "/tmp/velora_out.o", "/tmp/main");
}
