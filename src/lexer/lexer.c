#include "lexer.h"
#include <ctype.h>
#include <string.h>

static char advance(struct Lexer *l);
static void skip_whitespace(struct Lexer *l);
static struct Token make_tok(struct Lexer *l, enum TokenKind kind);

struct Lexer lexer_new(struct Arena *a, char *file_name, char *contents) {
  return (struct Lexer){
      .arena = a,
      .file_name = file_name,
      .contents = contents,
      .pos = 0,
      .current_span = (struct Span){
          .start_col = 1, .end_col = 1, .start_line = 1, .end_line = 1}};
}

static char peek(struct Lexer *l) { return l->contents[l->pos]; };
static char peek_next(struct Lexer *l) { return l->contents[l->pos + 1]; };

static char advance(struct Lexer *l) {
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

static void skip_whitespace(struct Lexer *l) {
  while (peek(l) == ' ' || peek(l) == '\t' || peek(l) == '\n' ||
         peek(l) == '\r')

    advance(l);
};

static struct Token make_tok(struct Lexer *l, enum TokenKind kind) {
  return (struct Token){
      .file_name = l->file_name, .span = l->current_span, .kind = kind};
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
    int start = l->pos;
    while (isdigit(peek(l))) {
      advance(l);
    }
    int length = l->pos - start;

    struct Token t = make_tok(l, TOK_INT_LITERAL);
    t.val = arena_strndup(l->arena, l->contents + start, length);
    return t;
  }

  if (isalpha(c) || c == '_') {
    int start = l->pos;
    while (isalpha(peek(l)) || isdigit(peek(l)) || peek(l) == '_') {
      advance(l);
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

  advance(l);
  return make_tok(l, TOK_ERROR);
}

