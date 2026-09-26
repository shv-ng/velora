#include "parser_internal.h"
#include <stdlib.h>

struct AstNode *parse_primary(struct ParserCtx *ctx) {

  if (ctx->current_token.kind == TOK_INT_LITERAL) {
    struct Token int_tok = ctx->current_token;
    parser_expect(ctx, TOK_INT_LITERAL);

    struct AstNode *node = astnode_new(ctx, AST_INT_LITERAL);
    node->as.int_literal.value = atoll(int_tok.val);
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

  struct Error err = {
      .kind = ERR_SYNTAX,
      .span = ctx->current_token.span,
      .as.syntax.expected = "expression",
      .as.syntax.found = token_kind_str(ctx->current_token.kind),
  };

  print_error(err, ctx->lexer->file_name, ctx->lexer->contents);
  ctx->error_count++;

  parser_synchronise(ctx);
  return NULL;
}
