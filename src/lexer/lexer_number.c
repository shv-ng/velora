#include "lexer_internal.h"
#include <ctype.h>

struct Token lexer_number(struct LexerCtx *ctx) {
  int start = ctx->pos;

  while (isdigit(lexer_peek(ctx))) {
    lexer_advance(ctx);
  }
  int length = ctx->pos - start;

  struct Token t = lexer_make_tok(ctx, TOK_INT_LITERAL);
  t.val = arena_strndup(ctx->arena, ctx->contents + start, length);
  return t;
}
