#include "codegen_internal.h"

void codegen_program(struct CodegenCtx *ctx, struct AstNode *root) {
  for (int i = 0; i < root->as.program.count; i++) {
    struct AstNode *decl = root->as.program.declaration[i];
    codegen_node(ctx, decl);
  }
  if (ctx->error_count == 0) {
    codegen_compile_binary(ctx);
  }
}
