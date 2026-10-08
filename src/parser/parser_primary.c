#include "parser_internal.h"
#include <stdlib.h>

struct AstNode *parse_primary(struct ParserCtx *ctx) {
  struct Token curr = ctx->current_token;

  if (curr.kind == TOK_COMMENT){
    return NULL;
  }

  if (curr.kind == TOK_KW_IF) {
    return parse_if_else_expression(ctx);
  }

  if (curr.kind == TOK_TRUE || curr.kind == TOK_FALSE) {
    parser_advance(ctx);
    struct AstNode *node = astnode_new(ctx, AST_BOOL_LITERAL);

    node->span = curr.span;
    node->as.bool_literal.is_true = curr.kind == TOK_TRUE;

    return node;
  }

  if (curr.kind == TOK_INT_LITERAL) {
    parser_advance(ctx);

    struct AstNode *node = astnode_new(ctx, AST_INT_LITERAL);

    node->as.int_literal.raw = curr.val;
    node->span = curr.span;

    return node;
  }

  if (curr.kind == TOK_LPAREN) {
    parser_advance(ctx);

    struct AstNode *expr = parse_expression(ctx, 0);

    parser_expect(ctx, TOK_RPAREN);
    return expr;
  }

  if (curr.kind == TOK_IDENTIFIER) {
    struct AstNode *node = astnode_new(ctx, AST_IDENTIFIER);
    node->as.identifer.name = curr.val;
    node->span = curr.span;

    parser_advance(ctx);

    return node;
  }

  emit_error(ctx->err, curr.span, ERR_EXPECTED_FOUND, "expression",
             token_kind_str(curr.kind));

  parser_synchronise(ctx);
  return NULL;
}
