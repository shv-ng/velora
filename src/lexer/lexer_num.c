#include "lexer_internal.h"
#include <ctype.h>

struct Token lexer_num(struct Lexer *l) {
  int start = l->pos;
  while (isdigit(peek(l))) {
    advance(l);
  }
  int length = l->pos - start;

  struct Token t = make_tok(l, TOK_INT_LITERAL);
  t.val = arena_strndup(l->arena, l->contents + start, length);
  return t;
}
