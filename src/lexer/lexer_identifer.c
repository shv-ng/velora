#include "lexer_internal.h"
#include <ctype.h>
#include <string.h>

struct Token lexer_identifier(struct Lexer *l) {
  int start = l->pos;
  while (isalpha(peek(l)) || isdigit(peek(l)) || peek(l) == '_') {
    lexer_advance(l);
  }
  int length = l->pos - start;

  struct Token t = make_tok(l, TOK_IDENTIFIER);
  t.val = arena_strndup(l->arena, l->contents + start, length);

  static struct Keyword keywords[] = {
      {"return", TOK_KW_RETURN},
      {"fn", TOK_KW_FUNC},
      {NULL, 0},
  };

  for (int k = 0; keywords[k].word; k++) {
    if (strcmp(t.val, keywords[k].word) == 0) {
      t.kind = keywords[k].kind;
      break;
    }
  }
  return t;
}
