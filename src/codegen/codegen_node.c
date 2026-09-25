#include "codegen.h"
#include "codegen_internal.h"
#include <llvm-c/Analysis.h>
#include <llvm-c/TargetMachine.h>

void codegen_node(struct CodegenCtx *ctx, struct AstNode *node) {

  switch (node->kind) {
  case AST_PROGRAM:
    codegen_program(ctx, node);
    break;
  case AST_FUNCTION_DECL:
    codegen_func(ctx, node);
    break;
  case AST_VAR_DECL:
    codegen_var_decl(ctx, node);
    break;
  case AST_BLOCK_DECL:
    codegen_block(ctx, node);
    break;
  case AST_RETURN_STMT:
    codegen_return(ctx, node);
    break;
  case AST_ASSIGNMENT:
  case AST_TYPE_UNKNOWN:
  case AST_TYPE_NAMED:
  case AST_EXPR_STMT:
  case AST_INT_LITERAL:
  case AST_IDENTIFIER:
  case AST_BINARY_EXPR:
  case AST_UNARY_EXPR:
    ctx->error_count++;
    struct Error err = {.span = node->span,
                        .kind = ERR_CODEGEN,
                        .as.codegen.message = "unhandled node in codegen"};

    print_error(err, ctx->file_name, ctx->contents);
    break;
  }
}
