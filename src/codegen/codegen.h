#pragma once

#include "../sema/sema.h"
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

// initialise CodegenCtx from SemaCtx that use everywhere by passing as ctx
struct CodegenCtx codegen_new(struct SemaCtx *sema);

// node dispatcher, for codegen recursively from a given node
void codegen_node(struct CodegenCtx *ctx, struct AstNode *node);

// clean up memory, and llvm stuffs
void codegen_free(struct CodegenCtx *ctx);
