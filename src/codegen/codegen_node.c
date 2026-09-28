#include "codegen.h"
#include "codegen_internal.h"
#include <llvm-c/Analysis.h>
#include <llvm-c/TargetMachine.h>

void codegen_node(struct CodegenCtx *ctx, struct AstNode *node) {

  switch (node->kind) {
  case AST_PROGRAM:
    codegen_program(ctx, node);
    break;
  case AST_FUNCTION_DECLARATION:
    codegen_function_declaration(ctx, node);
    break;
  case AST_VARIABLE_DECLARATION:
    codegen_variable_declaration(ctx, node);
    break;
  case AST_BLOCK_DECLARATION:
    codegen_block(ctx, node);
    break;
  case AST_RETURN_STATEMENT:
    codegen_return_statement(ctx, node);
    break;
  case AST_ASSIGNMENT:
    codegen_assignment(ctx, node);
    break;
  case AST_TYPE_UNKNOWN:
  case AST_TYPE_NAMED:
  case AST_EXPRESSION_STATEMENT:
  case AST_INT_LITERAL:
  case AST_IDENTIFIER:
  case AST_BINARY_EXPRESSION:
  case AST_UNARY_EXPRESSION:
    emit_error(ctx->err, NO_SPAN, ERR_CODEGEN, "unhandled node in codegen");
    break;
  }
}
