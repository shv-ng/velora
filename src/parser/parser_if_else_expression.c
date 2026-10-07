#include "parser.h"
#include "parser_internal.h"

struct AstNode *parse_if_else_expression(struct ParserCtx *ctx) {

  struct Span start = ctx->current_token.span;
  parser_expect(ctx, TOK_KW_IF);

  struct AstNode *node = astnode_new(ctx, AST_IF_ELSE_EXPRESSION);
  node->as.if_else_expression.condition = parse_expression(ctx, 0);

  node->as.if_else_expression.if_block = parse_block_declaration(ctx, NULL);

  if (ctx->current_token.kind == TOK_KW_ELSE) {
    parser_advance(ctx);
    if (ctx->current_token.kind == TOK_KW_IF) {
      node->as.if_else_expression.else_block = parse_if_else_expression(ctx);
    } else {
      node->as.if_else_expression.else_block =
          parse_block_declaration(ctx, NULL);
    }
  }

  struct Span end = node->as.if_else_expression.if_block->span;

  node->span = merge_span(start, end);

  return node;
}
