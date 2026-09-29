#include "parser_internal.h"

struct AstNode *parse_type(struct ParserCtx *ctx) {
  struct Span start = ctx->current_token.span;

  struct AstNode *type = astnode_new(ctx, AST_TYPE_UNKNOWN);

  switch (ctx->current_token.kind) {
  case TOK_IDENTIFIER: {
    struct Token name_tok = ctx->current_token;
    parser_expect(ctx, TOK_IDENTIFIER);

    type->kind = AST_TYPE_NAMED;
    type->as.type_named.name = name_tok.val;
    break;
  }
  case TOK_I8:
  case TOK_I32:
  case TOK_BOOL:
    type->kind = AST_TYPE_NAMED;
    type->as.type_named.name = token_kind_str(ctx->current_token.kind);
    break;
  default:
    emit_error(ctx->err, ctx->current_token.span, ERR_UNEXPECTED,
               token_kind_str(ctx->current_token.kind));
    parser_advance(ctx);
    break;
  }

  struct Span end = ctx->current_token.span;
  type->span = merge_span(start, end);
  return type;
}
