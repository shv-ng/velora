#include "lexer.h"
#include <ctype.h>
#include <string.h>

static char peek(struct Lexer *l);
static char peek_next(struct Lexer *l);
static char advance(struct Lexer *l);
static void skip_whitespace(struct Lexer *l);
static struct Token make_tok(struct Lexer *l, enum TokenKind kind);

char *kind_str(enum TokenKind kind) {
  switch (kind) {
  case TOK_EOF:
    return "End of file";
  case TOK_SEMICOLON:
    return "';'";
  case TOK_ERROR:
    return "Unknown token";
  case TOK_COLON:
    return "':'";
  case TOK_LPARAN:
    return "'('";
  case TOK_RPARAN:
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

struct Lexer lexer_init(char *file, char *src) {
  return (struct Lexer){
      .file = file, .src = src, .pos = 0, .line = 1, .col = 1};
}

static char peek(struct Lexer *l) { return l->src[l->pos]; };
static char peek_next(struct Lexer *l) { return l->src[l->pos + 1]; };

static char advance(struct Lexer *l) {
  char c = peek(l);
  if (c == '\n' || c == '\r') {
    l->line++;
    l->col = 1;
    if (c == '\r' && peek_next(l) == '\n') {
      l->pos++;
    }
  } else {
    l->col++;
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
      .file_name = l->file, .col = l->col, .line = l->line, .kind = kind};
}

struct Token next_token(struct Lexer *l) {
  skip_whitespace(l);

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
    return make_tok(l, TOK_LPARAN);
  case ')':
    advance(l);
    return make_tok(l, TOK_RPARAN);
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
    t.val = strndup(l->src + start, length);
    return t;
  }

  if (isalpha(c) || c == '_') {
    int start = l->pos;
    while (isalpha(peek(l)) || isdigit(peek(l)) || peek(l) == '_') {
      advance(l);
    }
    int length = l->pos - start;

    struct Token t = make_tok(l, TOK_IDENTIFIER);
    t.val = strndup(l->src + start, length);

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
