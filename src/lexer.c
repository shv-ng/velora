#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static char peek(struct Lexer *l);
static char peek_next(struct Lexer *l);
static char advance(struct Lexer *l);
static void skip_whitespace(struct Lexer *l);
static struct Token make_tok(struct Lexer *l, enum TokenKind kind);

char *token_kind_str(enum TokenKind kind) {
  switch (kind) {
  case TOK_EOF:
    return "end of file";
  case TOK_SEMICOLON:
    return "';'";
  case TOK_ERROR:
    return "unknown token";
  case TOK_COLON:
    return "':'";
  case TOK_LPAREN:
    return "'('";
  case TOK_RPAREN:
    return "')'";
  case TOK_LBRACE:
    return "'{'";
  case TOK_RBRACE:
    return "'}'";
  case TOK_STR_LITERAL:
    return "str literal";
  case TOK_IDENTIFIER:
    return "identifier";
  case TOK_INT_LITERAL:
    return "int literal";
  case TOK_KW_RETURN:
    return "'return' keyword";
  case TOK_KW_FUNC:
    return "'fn' Keyword";
  }
}

struct Span merge_span(struct Span s1, struct Span s2) {
  struct Span span;
  span.start_col = s1.start_col < s2.start_col ? s1.start_col : s2.start_col;
  span.end_col = s1.end_col > s2.end_col ? s1.end_col : s2.end_col;

  span.start_line =
      s1.start_line < s2.start_line ? s1.start_line : s2.start_line;
  span.end_line = s1.end_line > s2.end_line ? s1.end_line : s2.end_line;

  return span;
}

struct Lexer lexer_new(char *file_name, char *contents) {
  return (struct Lexer){
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
  }

  if (isdigit(c)) {
    int start = l->pos;
    while (isdigit(peek(l))) {
      advance(l);
    }
    int length = l->pos - start;

    struct Token t = make_tok(l, TOK_INT_LITERAL);
    t.val = strndup(l->contents + start, length);
    return t;
  }

  if (isalpha(c) || c == '_') {
    int start = l->pos;
    while (isalpha(peek(l)) || isdigit(peek(l)) || peek(l) == '_') {
      advance(l);
    }
    int length = l->pos - start;

    struct Token t = make_tok(l, TOK_IDENTIFIER);
    t.val = strndup(l->contents + start, length);

    static struct Keyword keywords[] = {
        {"return", TOK_KW_RETURN},
        {"fn", TOK_KW_FUNC},
        {NULL, 0},
    };

    for (int k = 0; keywords[k].word; k++) {
      if (strcmp(t.val, keywords[k].word) == 0) {
        t.kind = keywords[k].kind;
        free(t.val);
        break;
      }
    }
    return t;
  }

  advance(l);
  return make_tok(l, TOK_ERROR);
}
