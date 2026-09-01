#include "lexer_internal.h"
#include <ctype.h>

struct Lexer lexer_new(struct Arena *a, char *file_name, char *contents) {
  return (struct Lexer){
      .arena = a,
      .file_name = file_name,
      .contents = contents,
      .pos = 0,
      .current_span = (struct Span){
          .start_col = 1, .end_col = 1, .start_line = 1, .end_line = 1}};
}

struct Token next_token(struct Lexer *l) {
  skip_whitespace(l);

  // new token start when whitespace get skipped and old token ends
  l->current_span.start_col = l->current_span.end_col;
  l->current_span.start_line = l->current_span.end_line;

  char c = peek(l);

  switch (c) {
  case '\0':
    return make_tok(l, TOK_EOF);
  case ';':
    advance(l);
    return make_tok(l, TOK_SEMICOLON);
  case ':':
    advance(l);
    return make_tok(l, TOK_COLON);
  case '(':
    advance(l);
    return make_tok(l, TOK_LPAREN);
  case ')':
    advance(l);
    return make_tok(l, TOK_RPAREN);
  case '{':
    advance(l);
    return make_tok(l, TOK_LBRACE);
  case '}':
    advance(l);
    return make_tok(l, TOK_RBRACE);
  case '+':
    advance(l);
    return make_tok(l, TOK_PLUS);
  case '-':
    advance(l);
    return make_tok(l, TOK_MINUS);
  case '*':
    advance(l);
    return make_tok(l, TOK_STAR);
  case '/':
    advance(l);
    return make_tok(l, TOK_SLASH);
  case '%':
    advance(l);
    return make_tok(l, TOK_MODULO);
  case '&':
    advance(l);
    return make_tok(l, TOK_AMPERSAND);
  case '|':
    advance(l);
    return make_tok(l, TOK_PIPE);
  case '^':
    advance(l);
    return make_tok(l, TOK_CARET);
  case '~':
    advance(l);
    return make_tok(l, TOK_TIDLE);
  case '!':
    advance(l);
    return make_tok(l, TOK_BANG);
  case '<': {
    advance(l);
    c = peek(l);
    if (c == '<') {
      advance(l);
      return make_tok(l, TOK_LEFT_SHIFT);
    }
    return make_tok(l, TOK_LEFT_ARROW);
  }
  case '>': {
    advance(l);
    c = peek(l);
    if (c == '>') {
      advance(l);
      return make_tok(l, TOK_RIGHT_SHIFT);
    }
    return make_tok(l, TOK_RIGHT_ARROW);
  }
  }

  if (isdigit(c)) {
    return lexer_num(l);
  }

  if (isalpha(c) || c == '_') {
    return lexer_identifier(l); // it'll also handle keywords
  }

  advance(l);
  return make_tok(l, TOK_ERROR);
}
