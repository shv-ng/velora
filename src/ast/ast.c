#include "ast.h"
#include <stdio.h>

static void indentation(int indent) {
  for (int i = 0; i < indent; i++)
    printf("  ");
}

void print_ast(struct AstNode *node, int indent) {
  if (!node)
    return;

  indentation(indent);

  switch (node->kind) {
  case AST_PROGRAM: {
    printf("AstProgram: (resolved_type: %s)\n", type_str(node->resolved_type));
    for (int i = 0; i < node->as.program.count; i++) {
      print_ast(node->as.program.declaration[i], indent + 1);
    }
    break;
  }
  case AST_FUNCTION_DECL:
    printf("AstFunctionDecl: (name: %s, resolved_type: %s)\n",
           node->as.function.name, type_str(node->resolved_type));
    print_ast(node->as.function.return_type, indent + 1);
    print_ast(node->as.function.block, indent + 1);
    break;
  case AST_TYPE_UNKNOWN:
    printf("AstUnknownType: \n");
    break;
  case AST_TYPE_NAMED:
    printf("AstTypeNamed: (name: %s, resolved_type: %s)\n",
           node->as.type_named.name, type_str(node->resolved_type));
    break;
  case AST_BLOCK_DECL:
    printf("AstBlockDecl: (name: %s, resolved_type: %s)\n", node->as.block.name,
           type_str(node->resolved_type));
    for (int i = 0; i < node->as.block.count; i++) {
      print_ast(node->as.block.statements[i], indent + 1);
    }
    if (node->as.block.trailing_expr) {
      print_ast(node->as.block.trailing_expr, indent + 1);
    }
    break;
  case AST_RETURN_STMT:
    printf("AstReturnStmt: (resolved_type: %s)\n",
           type_str(node->resolved_type));
    print_ast(node->as.return_stmt.expr, indent + 1);
    break;
  case AST_INT_LITERAL:
    printf("AstIntLiteral: (value: %lld, resolved_type: %s)\n",
           node->as.int_literal.value, type_str(node->resolved_type));
    break;
  case AST_EXPR_STMT:
    printf("AstExprStmt: (resolved_type: %s)\n", type_str(node->resolved_type));
    print_ast(node->as.expr_stmt.expr, indent + 1);
    break;
  }
}
