#include "lexer.h"
#include "lexer_internal.h"

struct Token lexer_comment(struct LexerCtx *ctx) {
  int start = ctx->pos;
  while (lexer_peek(ctx) != '\n' && lexer_peek(ctx) != '\r') {
    lexer_advance(ctx);
  }
  int length = ctx->pos - start;

  struct Token t = lexer_make_tok(ctx, TOK_COMMENT);
  t.val = arena_strndup(ctx->arena, ctx->contents + start, length);
  return t;
}
