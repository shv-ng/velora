#pragma once

#include "codegen.h"
#include <llvm-c/TargetMachine.h>
#include <llvm-c/Types.h>

// get llvm type from ast type, assign at sema stage
LLVMTypeRef type_to_llvm(struct CodegenCtx *ctx, struct Type *type);

LLVMValueRef codegen_binary_expression(struct CodegenCtx *ctx,
                                       struct AstNode *node);
LLVMValueRef codegen_expression(struct CodegenCtx *ctx, struct AstNode *node);
LLVMValueRef codegen_identifier(struct CodegenCtx *ctx, struct AstNode *node);
LLVMValueRef codegen_int_literal(struct CodegenCtx *ctx, struct AstNode *node);
LLVMValueRef codegen_unary_expression(struct CodegenCtx *ctx,
                                      struct AstNode *node);
void codegen_block(struct CodegenCtx *ctx, struct AstNode *node);
void codegen_function_declaration(struct CodegenCtx *ctx, struct AstNode *node);
void codegen_node(struct CodegenCtx *ctx, struct AstNode *node);
void codegen_program(struct CodegenCtx *ctx, struct AstNode *node);
void codegen_return_statement(struct CodegenCtx *ctx, struct AstNode *node);
void codegen_variable_declaration(struct CodegenCtx *ctx, struct AstNode *node);

LLVMTargetMachineRef codegen_create_machine(struct CodegenCtx *ctx);
void codegen_compile_binary(struct CodegenCtx *ctx);
bool codegen_emit_object(struct CodegenCtx *ctx, LLVMTargetMachineRef machine,
                         const char *path);

bool codegen_link_binary(struct CodegenCtx *ctx, const char *obj_path,
                         const char *out_path);
