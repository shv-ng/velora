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
    lexer_advance(l);
    return make_tok(l, TOK_SEMICOLON);
  case ':':
    lexer_advance(l);
    return make_tok(l, TOK_COLON);
  case '(':
    lexer_advance(l);
    return make_tok(l, TOK_LPAREN);
  case ')':
    lexer_advance(l);
    return make_tok(l, TOK_RPAREN);
  case '{':
    lexer_advance(l);
    return make_tok(l, TOK_LBRACE);
  case '}':
    lexer_advance(l);
    return make_tok(l, TOK_RBRACE);
  case '+':
    lexer_advance(l);
    return make_tok(l, TOK_PLUS);
  case '-':
    lexer_advance(l);
    return make_tok(l, TOK_MINUS);
  case '*':
    lexer_advance(l);
    return make_tok(l, TOK_STAR);
  case '/':
    lexer_advance(l);
    return make_tok(l, TOK_SLASH);
  case '%':
    lexer_advance(l);
    return make_tok(l, TOK_MODULO);
  case '&':
    lexer_advance(l);
    return make_tok(l, TOK_AMPERSAND);
  case '|':
    lexer_advance(l);
    return make_tok(l, TOK_PIPE);
  case '^':
    lexer_advance(l);
    return make_tok(l, TOK_CARET);
  case '~':
    lexer_advance(l);
    return make_tok(l, TOK_TIDLE);
  case '!':
    lexer_advance(l);
    return make_tok(l, TOK_BANG);
  case '<': {
    lexer_advance(l);
    c = peek(l);
    if (c == '<') {
      lexer_advance(l);
      return make_tok(l, TOK_LEFT_SHIFT);
    }
    return make_tok(l, TOK_LEFT_ARROW);
  }
  case '>': {
    lexer_advance(l);
    c = peek(l);
    if (c == '>') {
      lexer_advance(l);
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

  lexer_advance(l);
  return make_tok(l, TOK_ERROR);
}
