#include "lexer_internal.h"
#include <ctype.h>
#include <string.h>

struct Reserved {
  const char *word;
  enum TokenKind kind;
};

static struct Reserved reserved[] = {
    {"return", TOK_KW_RETURN},
    {"fn", TOK_KW_FUNC},
    {"if", TOK_KW_IF},
    {"else", TOK_KW_ELSE},
    {"true", TOK_TRUE},
    {"false", TOK_FALSE},
    {NULL, 0},
};

// return either identifier (it later could be any fn name, var name, type etc),
// or keywords from listed keywords
struct Token lexer_identifier(struct LexerCtx *ctx) {
  int start = ctx->pos;

  while (isalpha(lexer_peek(ctx)) || isdigit(lexer_peek(ctx)) ||
         lexer_peek(ctx) == '_') {
    lexer_advance(ctx);
  }
  int length = ctx->pos - start;

  struct Token t = lexer_make_tok(ctx, TOK_IDENTIFIER);
  t.val = arena_strndup(ctx->arena, ctx->contents + start, length);

  for (int k = 0; reserved[k].word; k++) {
    if (strcmp(t.val, reserved[k].word) == 0) {
      t.kind = reserved[k].kind;
      break;
    }
  }
  return t;
}
