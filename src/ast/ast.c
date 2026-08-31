#include "ast.h"
#include "ast_binary_expr.h"
#include "ast_unary_expr.h"
#include <stdio.h>

static char *unaryop_to_str(enum UnaryOp op) {
  switch (op) {
  case OP_NEGATIVE:
    return "-";
  case OP_BITWISE_NOT:
    return "~";
  case OP_UNKNOWN_UNARY_OP:
    return "unknown";
  }
}
static char *binaryop_to_str(enum BinaryOp op) {
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
  case OP_UNKNOWN_BINARY_OP:
    return "unknown";
  }
}

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
  case AST_BINARY_EXPR:
    printf("AstBinaryExpr: (op: %s, resolved_type: %s)\n",
           binaryop_to_str(node->as.binary_expr.op),
           type_str(node->resolved_type));
    print_ast(node->as.binary_expr.left, indent + 1);
    print_ast(node->as.binary_expr.right, indent + 1);
    break;

  case AST_UNARY_EXPR:
    printf("AstUnaryExpr: (op: %s, is_prefix: %d, resolved_type: %s)\n",
           unaryop_to_str(node->as.unary_expr.op),
           node->as.unary_expr.is_prefix, type_str(node->resolved_type));
    print_ast(node->as.unary_expr.expr, indent + 1);
    break;
  }
}
