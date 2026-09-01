#include "lexer_internal.h"

char peek(struct Lexer *l) { return l->contents[l->pos]; };
char peek_next(struct Lexer *l) { return l->contents[l->pos + 1]; };

char advance(struct Lexer *l) {
  char c = peek(l);
  if (c == '\n' || c == '\r') {

    l->current_span.end_line++;
    l->current_span.end_col = 1;

    if (c == '\r' && peek_next(l) == '\n') {
      l->pos++;
    }
  } else {
    l->current_span.end_col++;
  }

  l->pos++;
  return c;
};

void skip_whitespace(struct Lexer *l) {
  while (peek(l) == ' ' || peek(l) == '\t' || peek(l) == '\n' ||
         peek(l) == '\r')

    advance(l);
};

struct Token make_tok(struct Lexer *l, enum TokenKind kind) {
  return (struct Token){
      .file_name = l->file_name, .span = l->current_span, .kind = kind};
}
