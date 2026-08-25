#pragma once

#include "../token/token.h"
#include "../utils/arena.h"
#include <stdint.h>
#include <sys/types.h>

struct Lexer {
  char *contents;
  char *file_name;
  int pos;
  struct Arena *arena;
  struct Span current_span;
};

struct Lexer lexer_new(struct Arena *a, char *file, char *src);

struct Token next_token(struct Lexer *l);
