#pragma once

#include "codegen.h"
#include <llvm-c/Types.h>

LLVMTypeRef type_to_llvm(struct CodegenCtx *ctx, struct Type *type);
LLVMValueRef codegen_binary_expression(struct CodegenCtx *ctx, struct AstNode *node);
LLVMValueRef codegen_expression(struct CodegenCtx *ctx, struct AstNode *node);
LLVMValueRef codegen_identifier(struct CodegenCtx *ctx, struct AstNode *node);
LLVMValueRef codegen_int_literal(struct CodegenCtx *ctx, struct AstNode *node);
LLVMValueRef codegen_unary_expression(struct CodegenCtx *ctx, struct AstNode *node);
void codegen_block(struct CodegenCtx *ctx, struct AstNode *node);
void codegen_compile_binary(struct CodegenCtx *ctx);
void codegen_function_declaration(struct CodegenCtx *ctx, struct AstNode *node);
void codegen_node(struct CodegenCtx *ctx, struct AstNode *node);
void codegen_program(struct CodegenCtx *ctx, struct AstNode *node);
void codegen_return_statement(struct CodegenCtx *ctx, struct AstNode *node);
void codegen_variable_declaration(struct CodegenCtx *ctx, struct AstNode *node);
