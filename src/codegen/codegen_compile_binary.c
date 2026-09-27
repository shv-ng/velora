#include "codegen.h"
#include "codegen_internal.h"
#include <llvm-c/Analysis.h>
#include <llvm-c/Core.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/Types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

bool codegen_link_binary(struct CodegenCtx *ctx, const char *obj_path,
                         const char *out_path) {

  // find available linkers
  // const char *linkers[] = {"clang", "gcc", "cc", NULL};
  const char *linker = "clang";

  // exact command, but in array
  const char *args[] = {linker,    obj_path, "-o", out_path,
                        "-static", "-O2",    NULL};

  // split into 2 process
  pid_t pid = fork();

  // 0 means child
  if (pid == 0) {

    // it'll find linker, and execute it
    execvp(linker, (char *const *)args);

    // if execvp fail, only then it'll called
    _exit(1);
  }

  // parent waits to child to fix the task
  int wstatus;
  waitpid(pid, &wstatus, 0);

  // check result, isn't it fails
  if (!WIFEXITED(wstatus) || WEXITSTATUS(wstatus) != 0) {
    ctx->error_count += 1;
    struct Error error = {.kind = ERR_CODEGEN,
                          .as.codegen.message = "linking failed"};

    print_error(error, ctx->file_name, ctx->contents);
    return false;
  }

  return true;
}

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
