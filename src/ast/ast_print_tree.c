#include "ast.h"
#include <stdio.h>

static char *ast_unaryop_to_str(enum UnaryOperator op) {
  switch (op) {
  case OP_NEGATIVE:
    return "-";
  case OP_BITWISE_NOT:
    return "~";
  case OP_UNKNOWN_UNARY_OP:
    return "unknown";
  }
}
static char *ast_binaryop_to_str(enum BinaryOperator op) {
  switch (op) {
  case OP_ADD:
    return "+";
  case OP_SUB:
    return "-";
  case OP_MULIPLY:
    return "*";
  case OP_DIVIDE:
    return "/";
  case OP_MODULO:
    return "%";
  case OP_LEFT_SHIFT:
    return "<<";
  case OP_RIGHT_SHIFT:
    return ">>";
  case OP_BITWISE_XOR:
    return "^";
  case OP_BITWISE_OR:
    return "|";
  case OP_BITWISE_AND:
    return "&";
  case OP_UNKNOWN_BINARY_OPERATOR:
    return "unknown";
  }
}

static void ast_indentation(int indent) {
  for (int i = 0; i < indent; i++)
    printf("  ");
}

void ast_print_tree(struct AstNode *node, int indent) {
  if (!node)
    return;

  ast_indentation(indent);

  switch (node->kind) {
  case AST_PROGRAM: {
    printf("AstProgram: (resolved_type: %s)\n", type_str(node->resolved_type));
    for (int i = 0; i < node->as.program.count; i++) {
      ast_print_tree(node->as.program.declaration[i], indent + 1);
    }
    break;
  }
  case AST_FUNCTION_DECLARATION:
    printf("AstFunctionDeclaration: (name: %s, resolved_type: %s)\n",
           node->as.function.name, type_str(node->resolved_type));
    ast_print_tree(node->as.function.return_type, indent + 1);
    ast_print_tree(node->as.function.block, indent + 1);
    break;
  case AST_TYPE_UNKNOWN:
    printf("AstUnknownType: \n");
    break;
  case AST_TYPE_NAMED:
    printf("AstTypeNamed: (name: %s, resolved_type: %s)\n",
           node->as.type_named.name, type_str(node->resolved_type));
    break;
  case AST_BLOCK_DECLARAION:
    printf("AstBlockDeclaration: (name: %s, resolved_type: %s)\n",
           node->as.block.name, type_str(node->resolved_type));
    for (int i = 0; i < node->as.block.count; i++) {
      ast_print_tree(node->as.block.statements[i], indent + 1);
    }
    if (node->as.block.trailing_expression) {
      ast_print_tree(node->as.block.trailing_expression, indent + 1);
    }
    break;
  case AST_RETURN_STATEMENT:
    printf("AstReturnStatement: (resolved_type: %s)\n",
           type_str(node->resolved_type));
    ast_print_tree(node->as.return_statement.expression, indent + 1);
    break;
  case AST_INT_LITERAL:
    printf("AstIntLiteral: (value: %lld, resolved_type: %s)\n",
           node->as.int_literal.value, type_str(node->resolved_type));
    break;
  case AST_EXPRESSION_STATEMENT:
    printf("AstExpressionStatement: (resolved_type: %s)\n",
           type_str(node->resolved_type));
    ast_print_tree(node->as.expression_statement.expression, indent + 1);
    break;
  case AST_BINARY_EXPRESSION:
    printf("AstBinaryExpression: (op: %s, resolved_type: %s)\n",
           ast_binaryop_to_str(node->as.binary_expression.op),
           type_str(node->resolved_type));
    ast_print_tree(node->as.binary_expression.left, indent + 1);
    ast_print_tree(node->as.binary_expression.right, indent + 1);
    break;

  case AST_UNARY_EXPRESSION:
    printf("AstUnaryExpression: (op: %s, is_prefix: %d, resolved_type: %s)\n",
           ast_unaryop_to_str(node->as.unary_expression.op),
           node->as.unary_expression.is_prefix, type_str(node->resolved_type));
    ast_print_tree(node->as.unary_expression.expression, indent + 1);
    break;
  case AST_VARIABLE_DECLARATION:
    printf("AstVariableDeclaration: (name: %s, resolved_type: %s)\n",
           node->as.variable_declaration.name, type_str(node->resolved_type));
    ast_print_tree(node->as.variable_declaration.type, indent + 1);
    ast_print_tree(node->as.variable_declaration.expression, indent + 1);
    break;
  case AST_IDENTIFIER:
    printf("AstIdentifier: (name: %s, resolved_type: %s)\n",
           node->as.identifer.name, type_str(node->resolved_type));
    break;
  case AST_ASSIGNMENT:
    printf("AstAssignment: (resolved_type: %s)\n",
           type_str(node->resolved_type));
    ast_print_tree(node->as.assignment.lvalue, indent + 1);
    ast_print_tree(node->as.assignment.rvalue, indent + 1);
    break;
  }
}
