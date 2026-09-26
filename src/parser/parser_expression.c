#include "parser_internal.h"
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>

static enum UnaryOperator token_to_unaryOperator(enum TokenKind kind) {
  switch (kind) {
  case TOK_MINUS:
    return OP_NEGATIVE;
  case TOK_TIDLE:
    return OP_BITWISE_NOT;
  default:
    return OP_UNKNOWN_UNARY_OP;
  }
}

static enum BinaryOperator token_to_binaryOperator(enum TokenKind kind) {
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
    return OP_UNKNOWN_BINARY_OPERATOR;
  }
}

static int infix_bp(enum TokenKind kind) {
  switch (kind) {
  case TOK_STAR:
  case TOK_SLASH:
  case TOK_MODULO:
    return 20;
  case TOK_PLUS:
  case TOK_MINUS:
    return 15;
  case TOK_LEFT_SHIFT:
  case TOK_RIGHT_SHIFT:
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

struct AstNode *parse_expression(struct ParserCtx *ctx, int min_bp) {
  struct AstNode *left = NULL;

  if (ctx->current_token.kind == TOK_MINUS ||
      ctx->current_token.kind == TOK_TIDLE) {
    struct AstNode *node = astnode_new(ctx, AST_UNARY_EXPRESSION);

    node->as.unary_expression.op =
        token_to_unaryOperator(ctx->current_token.kind);
    struct Span prev_span = ctx->current_token.span;

    parser_advance(ctx);
    node->as.unary_expression.is_prefix = true;
    node->as.unary_expression.expression = parse_expression(ctx, 100);
    node->span =
        merge_span(prev_span, node->as.unary_expression.expression->span);

    left = node;
  } else {
    left = parse_primary(ctx);
  }

  for (;;) {
    int bp = infix_bp(ctx->current_token.kind);
    if (bp <= min_bp)
      break;
    enum BinaryOperator op = token_to_binaryOperator(ctx->current_token.kind);
    parser_advance(ctx);

    struct AstNode *right = parse_expression(ctx, bp);

    struct AstNode *expr = astnode_new(ctx, AST_BINARY_EXPRESSION);
    expr->as.binary_expression.left = left;
    expr->as.binary_expression.op = op;
    expr->as.binary_expression.right = right;
    expr->span = merge_span(left->span, right->span);

    left = expr;
  }
  return left;
}
