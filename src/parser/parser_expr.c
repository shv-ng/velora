#include "parser.h"
#include "parser_internal.h"
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>

static enum UnaryOp token_to_unaryOp(enum TokenKind kind) {
  switch (kind) {
  case TOK_MINUS:
    return OP_NEGATIVE;
  case TOK_TIDLE:
    return OP_BITWISE_NOT;
  default:
    return OP_UNKNOWN_UNARY_OP;
  }
}

static enum BinaryOp token_to_binaryOp(enum TokenKind kind) {
  switch (kind) {
  case TOK_PLUS:
    return OP_ADD;
  case TOK_MINUS:
    return OP_SUB;
  case TOK_STAR:
    return OP_MULIPLY;
  case TOK_SLASH:
    return OP_DIVIDE;
  case TOK_MODULO:
    return OP_MODULO;
  case TOK_AMPERSAND:
    return OP_BITWISE_AND;
  case TOK_PIPE:
    return OP_BITWISE_OR;
  case TOK_CARET:
    return OP_BITWISE_XOR;
  case TOK_LEFT_SHIFT:
    return OP_LEFT_SHIFT;
  case TOK_RIGHT_SHIFT:
    return OP_RIGHT_SHIFT;
  default:
    return OP_UNKNOWN_BINARY_OP;
  }
}

static int infix_bp(enum TokenKind kind) {
  switch (kind) {
  case TOK_STAR:
  case TOK_SLASH:
  case TOK_MODULO:
    return 20;
  case TOK_LEFT_SHIFT:
  case TOK_RIGHT_SHIFT:
    return 15;
  case TOK_PLUS:
  case TOK_MINUS:
    return 10;
  case TOK_CARET:
    return 5;
  case TOK_AMPERSAND:
    return 6;
  case TOK_PIPE:
    return 4;
  default:
    return -1;
  }
}

struct AstNode *parse_expr(struct Parser *p, int min_bp) {
  struct AstNode *left = NULL;

  if (p->current_token.kind == TOK_MINUS ||
      p->current_token.kind == TOK_TIDLE) {
    struct AstNode *node = astnode_new(p, AST_UNARY_EXPR);

    node->as.unary_expr.op = token_to_unaryOp(p->current_token.kind);

    advance(p);
    node->as.unary_expr.is_prefix = true;
    node->as.unary_expr.expr = parse_expr(p, 100);

    left = node;
  } else {
    left = parse_primary(p);
  }

  for (;;) {
    int bp = infix_bp(p->current_token.kind);
    if (bp <= min_bp)
      break;
    enum BinaryOp op = token_to_binaryOp(p->current_token.kind);
    advance(p);

    struct AstNode *right = parse_expr(p, bp);

    struct AstNode*expr = astnode_new(p, AST_BINARY_EXPR);
    expr->as.binary_expr.left = left;
    expr->as.binary_expr.op = op;
    expr->as.binary_expr.right = right;

    left = expr;
  }
  return left;
}
