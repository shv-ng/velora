#include "codegen.h"
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
      emit_error(ctx->err, NO_SPAN, ERR_CODEGEN, strerror(errno));
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
      emit_error(ctx->err, NO_SPAN, ERR_CODEGEN, "linker crashed");
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
      emit_error(ctx->err, NO_SPAN, ERR_CODEGEN,
                 "no linker found: install clang or gcc");
      return false;
    }

    if (code == 126) {
      emit_error(ctx->err, NO_SPAN, ERR_CODEGEN,
                 "permission error: '%s' doesn't have execute permission",
                 linker);
      return false;
    }

    // for error like file not found, invalid .o file etc etc,
    // linker already say what's wrong
    emit_error(ctx->err, NO_SPAN, ERR_CODEGEN,
               "linking failed (exit code %d) — see above for details", code);
    return false;
  }

  return false;
}
