#pragma once

#include "../token/token.h"
#include "../utils/arena.h"

struct LexerCtx {
  char *contents;
  char *file_name;
  int pos;
  struct Arena *arena;
  struct Span span;
};

struct LexerCtx lexer_new(struct Arena *a, char *file, char *src);

struct Token lexer_next_token(struct LexerCtx *ctx);
