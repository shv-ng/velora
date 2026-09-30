#include "parser_internal.h"

struct AstNode *parse_type(struct ParserCtx *ctx) {
  struct Token curr = ctx->current_token;
  struct Span start = curr.span;

  struct AstNode *type = astnode_new(ctx, AST_TYPE_UNKNOWN);

  switch (curr.kind) {
  case TOK_I8:
  case TOK_I32:
  case TOK_BOOL:
  case TOK_IDENTIFIER: {
    type->kind = AST_TYPE_NAMED;
    type->as.type_named.name = curr.val;
    parser_advance(ctx);
    break;
  }
  default:
    emit_error(ctx->err, curr.span, ERR_UNEXPECTED, token_kind_str(curr.kind));
    parser_advance(ctx);
    break;
  }

  struct Span end = curr.span;
  type->span = merge_span(start, end);
  return type;
}
