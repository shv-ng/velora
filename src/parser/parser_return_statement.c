#include "parser_internal.h"

struct AstNode *parse_return_statement(struct ParserCtx *ctx) {
  struct Span start = ctx->current_token.span;
  parser_expect(ctx, TOK_KW_RETURN);

  struct AstNode *return_stmt = astnode_new(ctx, AST_RETURN_STATEMENT);

  if (ctx->current_token.kind != TOK_SEMICOLON) {
    return_stmt->as.return_statement.expression = parse_expression(ctx, 0);
  }

  struct Span end = ctx->current_token.span;
  parser_expect(ctx, TOK_SEMICOLON);
  return_stmt->span = merge_span(start, end);

  return return_stmt;
}
