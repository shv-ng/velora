#include "parser_internal.h"

struct AstNode *parse_function_declaration(struct ParserCtx *ctx, char *name) {
  struct Span start = ctx->current_token.span;
  parser_expect(ctx, TOK_KW_FUNC);

  parser_expect(ctx, TOK_LPAREN);
  parser_expect(ctx, TOK_RPAREN);

  struct AstNode *return_type = parse_type(ctx);
  struct AstNode *block = parse_block_declaration(ctx, name);

  if (ctx->error_count > 0) {
    return NULL;
  }

  struct AstNode *func = astnode_new(ctx, AST_FUNCTION_DECLARATION);

  func->as.function.name = name;
  func->as.function.return_type = return_type;
  func->as.function.block = block;

  struct Span end = ctx->current_token.span;
  func->span = merge_span(start, end);
  return func;
}
