#include "../ast/ast.h"
#include <stdbool.h>

bool ast_all_type_resolve(struct AstNode *node) {
  if (!node) {
    return true;
  }

  if (node->resolved_type->kind == TYPE_UNKNOWN) {
    return false;
  }

  switch (node->kind) {
  case AST_PROGRAM: {
    for (int i = 0; i < node->as.program.count; i++) {
      if (!ast_all_type_resolve(node->as.program.declaration[i])) {
        return false;
      }
    }
    break;
  }
  case AST_FUNCTION_DECL:
    if (!ast_all_type_resolve(node->as.function.return_type) ||
        !ast_all_type_resolve(node->as.function.block)) {
      return false;
    }
    break;
  case AST_VAR_DECL:
    if (!ast_all_type_resolve(node->as.var_decl.type) ||
        !ast_all_type_resolve(node->as.var_decl.expr)) {
      return false;
    }
    break;
  case AST_ASSIGNMENT:
    if (!ast_all_type_resolve(node->as.assignment.lhs) ||
        !ast_all_type_resolve(node->as.assignment.rhs)) {
      return false;
    }
    break;
  case AST_TYPE_UNKNOWN:
    return false;
  case AST_TYPE_NAMED:
    break;
  case AST_BLOCK_DECL: {
    if (!ast_all_type_resolve(node->as.block.trailing_expr)) {
      return false;
    }

    for (int i = 0; i < node->as.block.count; i++) {
      if (!ast_all_type_resolve(node->as.block.statements[i])) {
        return false;
      }
    }
    break;
  }
  case AST_RETURN_STMT:
    if (!ast_all_type_resolve(node->as.return_stmt.expr)) {
      return false;
    }
    break;
  case AST_EXPR_STMT:
    if (!ast_all_type_resolve(node->as.expr_stmt.expr)) {
      return false;
    }
    break;
  case AST_INT_LITERAL:
    break;
  case AST_IDENTIFIER:
    break;
  case AST_BINARY_EXPR:
    if (!ast_all_type_resolve(node->as.binary_expr.left) ||
        !ast_all_type_resolve(node->as.binary_expr.right)) {
      return false;
    }
    break;
  case AST_UNARY_EXPR:
    if (!ast_all_type_resolve(node->as.unary_expr.expr)) {
      return false;
    }
    break;
  }
  return true;
}
