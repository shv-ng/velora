#include "parser.h"
#include <stddef.h>

struct ParserCtx parser_new(struct LexerCtx *lexer_ctx) {

  struct ErrorCtx *err =
      arena_malloc(lexer_ctx->arena, sizeof(struct ErrorCtx));

  err->file_name = lexer_ctx->file_name;
  err->content = lexer_ctx->contents;

  struct ParserCtx ctx = {
      .arena = lexer_ctx->arena,
      .lexer = lexer_ctx,
      .err = err,
  };

  ctx.current_token = lexer_next_token(lexer_ctx);
  ctx.next_token = lexer_next_token(lexer_ctx);

  ctx.err->count = 0;

  return ctx;
}
