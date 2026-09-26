#include "lexer_internal.h"

// return what char is currenty pos marking
char lexer_peek(struct LexerCtx *ctx) { return ctx->contents[ctx->pos]; };

// return what char is after pos mark
char lexer_peek_next(struct LexerCtx *ctx) {
  return ctx->contents[ctx->pos + 1];
};

// return current char and move forward, also can handle new line char like
// '\n', '\r\n', '\r'
char lexer_advance(struct LexerCtx *ctx) {
  char c = lexer_peek(ctx);
  if (c == '\n' || c == '\r') {

    ctx->span.end_line++;
    ctx->span.end_col = 1;

    if (c == '\r' && lexer_peek_next(ctx) == '\n') {
      ctx->pos++;
    }
  } else {
    ctx->span.end_col++;
  }

  ctx->pos++;
  return c;
};

void lexer_skip_whitespace(struct LexerCtx *ctx) {
  while (lexer_peek(ctx) == ' ' || lexer_peek(ctx) == '\t' ||
         lexer_peek(ctx) == '\n' || lexer_peek(ctx) == '\r')

    lexer_advance(ctx);
};

struct Token lexer_make_tok(struct LexerCtx *ctx, enum TokenKind kind) {
  return (struct Token){
      .file_name = ctx->file_name,
      .span = ctx->span,
      .kind = kind,
  };
}
