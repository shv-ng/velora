#include "codegen.h"
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/Core.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/Types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static void print_codegen_err(struct CodegenCtx *ctx, const char *msg) {
  ctx->error_count++;
  print_error(
      (struct Error){
          .kind = ERR_CODEGEN,
          .as.codegen.message = msg,
      },
      ctx->file_name, ctx->contents);
}

bool codegen_link_binary(struct CodegenCtx *ctx, const char *obj_path,
                         const char *out_path) {
  // find available linkers
  const char *linkers[] = {"clang", "gcc", "cc", NULL};
  for (int i = 0; linkers[i]; i++) {
    const char *linker = linkers[i];

    // exact command, but in array
    const char *args[] = {linker,    obj_path, "-o", out_path,
                          "-static", "-O2",    NULL};

    // split into 2 process
    pid_t pid = fork();

    if (pid == -1) {
      // errno tells exactly why fork failed
      print_codegen_err(ctx, strerror(errno));
      return false;
    }

    // 0 means child
    if (pid == 0) {

      // it'll find linker, and execute it
      execvp(linker, (char *const *)args);

      // if execvp fail, only then it'll called
      // 127 means command not found: just convention btw
      if (errno == ENOENT) {
        _exit(127);
      }
      // 126 means command found but don't have permission: just convention btw
      _exit(126);
    }

    // parent waits to child to fix the task
    int wstatus;
    waitpid(pid, &wstatus, 0);

    // check result, isn't it fails
    if (!WIFEXITED(wstatus)) {
      print_codegen_err(ctx, "linker crashed");
      return false;
    }

    int code = WEXITSTATUS(wstatus);
    // success
    if (code == 0) {
      return true;
    }

    if (linkers[i + 1] && (code == 127 || code == 128)) {
      continue;
    }

    if (code == 127) {
      print_codegen_err(ctx, "no linker found: install clang or gcc");
      return false;
    }

    if (code == 126) {
      char msg[256];
      snprintf(msg, sizeof(msg),
               "permission error: '%s' doesn't have execute permission",
               linker);
      print_codegen_err(ctx, msg);
      return false;
    }

    // for error like file not found, invalid .o file etc etc,
    // linker already say what's wrong
    char msg[256];
    snprintf(msg, sizeof(msg),
             "linking failed (exit code %d) — see above for details", code);
    print_codegen_err(ctx, msg);
    return false;
  }

  return false;
}
