#pragma once

#include "../ast/ast.h"
#include "../sema/sema.h"
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

struct CodegenCtx codegen_new(struct SemaCtx *sema);

void codegen_emit(struct CodegenCtx *ctx, struct AstNode *root);
void codegen_free(struct CodegenCtx *ctx);
