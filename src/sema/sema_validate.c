#include "sema_internal.h"

bool sema_check_type_resolve(struct SemaCtx *ctx, struct AstNode *node) {
  if (!node) {
    return true;
  }

  if (node->resolved_type->kind == TYPE_UNKNOWN) {
    ctx->error_count++;
    struct Error err = {
        .kind = ERR_UNRESOLVED_TYPE,
        .span = node->span,
    };
    print_error(err, ctx->file_name, ctx->contents);

    return false;
  }

  switch (node->kind) {
  case AST_PROGRAM: {
    for (int i = 0; i < node->as.program.count; i++) {
      if (!sema_check_type_resolve(ctx, node->as.program.declaration[i])) {
        return false;
      }
    }
    break;
  }
  case AST_FUNCTION_DECLARATION:
    if (!sema_check_type_resolve(ctx, node->as.function.return_type) ||
        !sema_check_type_resolve(ctx, node->as.function.block)) {
      return false;
    }
    break;
  case AST_VARIABLE_DECLARATION:
    if (!sema_check_type_resolve(ctx, node->as.variable_declaration.type) ||
        !sema_check_type_resolve(ctx, node->as.variable_declaration.expression)) {
      return false;
    }
    break;
  case AST_ASSIGNMENT:
    if (!sema_check_type_resolve(ctx, node->as.assignment.lvalue) ||
        !sema_check_type_resolve(ctx, node->as.assignment.rvalue)) {
      return false;
    }
    break;
  case AST_TYPE_UNKNOWN:
    return false;
  case AST_TYPE_NAMED:
    break;
  case AST_BLOCK_DECLARATION: {
    if (!sema_check_type_resolve(ctx, node->as.block.trailing_expression)) {
      return false;
    }

    for (int i = 0; i < node->as.block.count; i++) {
      if (!sema_check_type_resolve(ctx, node->as.block.statements[i])) {
        return false;
      }
    }
    break;
  }
  case AST_RETURN_STATEMENT:
    if (!sema_check_type_resolve(ctx, node->as.return_statement.expression)) {
      return false;
    }
    break;
  case AST_EXPRESSION_STATEMENT:
    if (!sema_check_type_resolve(ctx, node->as.expression_statement.expression)) {
      return false;
    }
    break;
  case AST_INT_LITERAL:
    break;
  case AST_IDENTIFIER:
    break;
  case AST_BINARY_EXPRESSION:
    if (!sema_check_type_resolve(ctx, node->as.binary_expression.left) ||
        !sema_check_type_resolve(ctx, node->as.binary_expression.right)) {
      return false;
    }
    break;
  case AST_UNARY_EXPRESSION:
    if (!sema_check_type_resolve(ctx, node->as.unary_expression.expression)) {
      return false;
    }
    break;
  }
  return true;
}
