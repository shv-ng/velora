#include "parser_internal.h"
#include <stdlib.h>

struct AstNode *parse_primary(struct ParserCtx *ctx) {

  if (ctx->current_token.kind == TOK_INT_LITERAL) {
    struct Token int_tok = ctx->current_token;
    parser_expect(ctx, TOK_INT_LITERAL);

    struct AstNode *node = astnode_new(ctx, AST_INT_LITERAL);

    node->as.int_literal.raw = int_tok.val;
    node->span = int_tok.span;

    return node;
  }

  if (ctx->current_token.kind == TOK_LPAREN) {
    parser_advance(ctx);

    struct AstNode *expr = parse_expression(ctx, 0);

    parser_expect(ctx, TOK_RPAREN);
    return expr;
  }

  if (ctx->current_token.kind == TOK_IDENTIFIER) {
    struct AstNode *node = astnode_new(ctx, AST_IDENTIFIER);
    node->as.identifer.name = ctx->current_token.val;
    node->span = ctx->current_token.span;

    parser_advance(ctx);

    return node;
  }

  emit_error(ctx->err, ctx->current_token.span, ERR_EXPECTED_FOUND,
             "expression", token_kind_str(ctx->current_token.kind));

  parser_synchronise(ctx);
  return NULL;
}
