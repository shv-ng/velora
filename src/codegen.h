#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "sema.h"
#include <llvm-c/Core.h>
#include <llvm-c/Types.h>

struct CodegenCtx {
  LLVMContextRef context;
  LLVMModuleRef module;
  LLVMBuilderRef builder;

  LLVMValueRef current_func;

  int error_count;

  char *contents;
  char *file_name;
};

struct CodegenCtx codegen_new();

void codegen_emit(struct CodegenCtx *ctx, struct AstNode *root);

#endif
