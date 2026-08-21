
#include "codegen.h"
#include "ast.h"
#include "error.h"
#include "sema.h"
#include "types.h"
#include <llvm-c/Analysis.h>
#include <llvm-c/Core.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/Types.h>
#include <stdlib.h>
#include <sys/wait.h>

void codegen_stmt(struct CodegenCtx *ctx, struct AstNode *node);

static LLVMTypeRef type_to_llvm(struct CodegenCtx *ctx, struct Type *type) {
  switch (type->kind) {
  case TYPE_I8:
    return LLVMInt8TypeInContext(ctx->context);
  case TYPE_I32:
    return LLVMInt32TypeInContext(ctx->context);
  case TYPE_VOID:
    return LLVMVoidTypeInContext(ctx->context);
  case TYPE_UNKNOWN:
  case TYPE_FUNC:
    return NULL;
  }
}

static LLVMValueRef codegen_int_literal(struct CodegenCtx *ctx,
                                        struct AstNode *node) {
  LLVMTypeRef t = type_to_llvm(ctx, node->resolved_type);
  return LLVMConstInt(t, (unsigned long long)node->as.int_literal.value, 1);
}

LLVMValueRef codegen_expr(struct CodegenCtx *ctx, struct AstNode *node) {
  switch (node->kind) {
  case AST_INT_LITERAL:
    return codegen_int_literal(ctx, node);
  default:
    ctx->error_count += 1;

    struct Error err = {.span = node->span,
                        .kind = ERR_CODEGEN,
                        .as.codegen = (struct ErrCodegen){
                            .message = "unhandled node kind in expr"}};

    print_error(err, ctx->file_name, ctx->contents);

    break;
  }
  return NULL;
}

void codegen_return(struct CodegenCtx *ctx, struct AstNode *node) {
  LLVMValueRef value = codegen_expr(ctx, node->as.return_stmt.expr);
  LLVMBuildRet(ctx->builder, value);
}

void codegen_block(struct CodegenCtx *ctx, struct AstNode *node) {
  for (int i = 0; i < node->as.block.count; i++) {
    codegen_stmt(ctx, node->as.block.statements[i]);
  }
}

void codegen_stmt(struct CodegenCtx *ctx, struct AstNode *node) {
  switch (node->kind) {
  case AST_BLOCK_DECL:
    codegen_block(ctx, node);
    break;
  case AST_RETURN_STMT:
    codegen_return(ctx, node);
    break;
  default:
    ctx->error_count += 1;

    struct Error err = {
        .span = node->span,
        .kind = ERR_CODEGEN,
        .as.codegen = (struct ErrCodegen){.message = "unhandled node in stmt"}};

    print_error(err, ctx->file_name, ctx->contents);

    break;
  }
}
void codegen_func(struct CodegenCtx *ctx, struct AstNode *node) {
  LLVMTypeRef ret_type = type_to_llvm(ctx, node->resolved_type);
  LLVMTypeRef func_type = LLVMFunctionType(ret_type, NULL, 0, 0);

  LLVMValueRef func =
      LLVMAddFunction(ctx->module, node->as.function.name, func_type);
  ctx->current_func = func;

  LLVMBasicBlockRef entry =
      LLVMAppendBasicBlockInContext(ctx->context, func, "entry");
  LLVMPositionBuilderAtEnd(ctx->builder, entry);

  codegen_stmt(ctx, node->as.function.block);
}

struct CodegenCtx codegen_new(struct SemaCtx *sema) {

  struct CodegenCtx ctx = {.file_name = sema->file_name,
                           .contents = sema->contents};

  ctx.context = LLVMContextCreate();
  ctx.module = LLVMModuleCreateWithNameInContext(ctx.file_name, ctx.context);
  ctx.builder = LLVMCreateBuilderInContext(ctx.context);

  ctx.error_count = 0;

  return ctx;
}

void codegen_binary(struct CodegenCtx *ctx) {
  // verify llvm mod
  char *err = NULL;
  if (LLVMVerifyModule(ctx->module, LLVMPrintMessageAction, &err)) {
    ctx->error_count += 1;
    struct Error error = {.kind = ERR_CODEGEN,
                          .as.codegen = (struct ErrCodegen){.message = err}};

    print_error(error, ctx->file_name, ctx->contents);
  }

  LLVMDisposeMessage(err);

  // get target triple: TODO: will change as per user given
  char *triple = LLVMGetDefaultTargetTriple();

  // initialise targets
  // LLVMInitializeAllTargetInfos();
  // LLVMInitializeAllTargets();
  // LLVMInitializeAllTargetMCs();
  // LLVMInitializeAllAsmParsers();
  // LLVMInitializeAllAsmPrinters();
  LLVMInitializeNativeTarget();
  LLVMInitializeNativeAsmPrinter();
  LLVMInitializeNativeAsmParser();

  // get target from triple
  LLVMTargetRef target;
  char *target_err = NULL;
  if (LLVMGetTargetFromTriple(triple, &target, &target_err)) {
    ctx->error_count += 1;
    struct Error error = {.kind = ERR_CODEGEN,
                          .as.codegen =
                              (struct ErrCodegen){.message = target_err}};

    print_error(error, ctx->file_name, ctx->contents);
  }
  LLVMDisposeMessage(target_err);

  // create target machine and config module
  LLVMTargetMachineRef machine = LLVMCreateTargetMachine(
      target, triple, "generic", "", LLVMCodeGenLevelDefault, LLVMRelocDefault,
      LLVMCodeModelDefault);

  LLVMTargetDataRef data_layout = LLVMCreateTargetDataLayout(machine);
  LLVMSetModuleDataLayout(ctx->module, data_layout);
  LLVMSetTarget(ctx->module, triple);
  LLVMDisposeTargetData(data_layout);
  LLVMDisposeMessage(triple);

  // emit object file
  char *emit_err = NULL;
  if (LLVMTargetMachineEmitToFile(machine, ctx->module, "/tmp/velora_out.o",
                                  LLVMObjectFile, &emit_err)) {
    ctx->error_count += 1;
    struct Error error = {.kind = ERR_CODEGEN,
                          .as.codegen =
                              (struct ErrCodegen){.message = emit_err}};

    print_error(error, ctx->file_name, ctx->contents);
  }
  LLVMDisposeMessage(emit_err);
  LLVMDisposeTargetMachine(machine);

  int status = system("clang /tmp/velora_out.o -o /tmp/main -static");

  if (status == -1) {
    ctx->error_count += 1;
    struct Error error = {
        .kind = ERR_CODEGEN,
        .as.codegen = (struct ErrCodegen){.message = "fail to run clang"}};

    print_error(error, ctx->file_name, ctx->contents);
  }
}

void codegen_emit(struct CodegenCtx *ctx, struct AstNode *root) {
  for (int i = 0; i < root->as.program.count; i++) {
    struct AstNode *decl = root->as.program.declaration[i];

    switch (decl->kind) {
    case AST_FUNCTION_DECL:
      codegen_func(ctx, decl);
      break;

    default:
      break;
    }
  }

  codegen_binary(ctx);
}

void codegen_free(struct CodegenCtx *ctx) {
  LLVMDisposeBuilder(ctx->builder);
  LLVMDisposeModule(ctx->module);
  LLVMContextDispose(ctx->context);
}
