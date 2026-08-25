#pragma once

#include "../error/error.h"
#include "codegen.h"

void codegen_func(struct CodegenCtx *ctx, struct AstNode *node);

void codegen_return(struct CodegenCtx *ctx, struct AstNode *node);
void codegen_block(struct CodegenCtx *ctx, struct AstNode *node);
void codegen_stmt(struct CodegenCtx *ctx, struct AstNode *node);

LLVMValueRef codegen_int_literal(struct CodegenCtx *ctx, struct AstNode *node);
LLVMValueRef codegen_expr(struct CodegenCtx *ctx, struct AstNode *node);

LLVMTypeRef type_to_llvm(struct CodegenCtx *ctx, struct Type *type);
void codegen_binary(struct CodegenCtx *ctx);
