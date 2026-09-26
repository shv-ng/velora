#include "parser.h"
#include <stddef.h>

struct ParserCtx parser_new(struct LexerCtx *lexer_ctx) {
  struct ParserCtx ctx = {
      .lexer = lexer_ctx,
      .error_count = 0,
      .arena = lexer_ctx->arena,
  };

  ctx.current_token = lexer_next_token(lexer_ctx);
  ctx.next_token = lexer_next_token(lexer_ctx);

  return ctx;
}
