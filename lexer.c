#include "lexer.h"
#include <ctype.h>
#include <string.h>

static char peek(Lexer *l);
static char peek_next(Lexer *l);
static char advance(Lexer *l);
static void skip_whitespace(Lexer *l);
static Token make_tok(Lexer *l, TokenKind kind);

char *kind_str(TokenKind kind) {
  static const char *names[] = {
#define X(name) #name,
      TOKEN_KINDS
#undef X
  };
  return (char *)names[kind];
}

Lexer lexer_init(char *file, char *src) {
  return (Lexer){.file = file, .src = src, .pos = 0, .line = 1, .col = 1};
}

static char peek(Lexer *l) { return l->src[l->pos]; };
static char peek_next(Lexer *l) { return l->src[l->pos + 1]; };

static char advance(Lexer *l) {
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

static void skip_whitespace(Lexer *l) {
  while (peek(l) == ' ' || peek(l) == '\t' || peek(l) == '\n' ||
         peek(l) == '\r')

    advance(l);
};

static Token make_tok(Lexer *l, TokenKind kind) {
  return (Token){
      .file_name = l->file, .col = l->col, .line = l->line, .kind = kind};
}

Token next_token(Lexer *l) {
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

    Token t = make_tok(l, TOK_INT_LIT);
    t.val = strndup(l->src + start, length);
    return t;
  }

  if (isalpha(c) || c == '_') {
    int start = l->pos;
    while (isalpha(peek(l)) || isdigit(peek(l)) || peek(l) == '_') {
      advance(l);
    }
    int length = l->pos - start;

    Token t = make_tok(l, TOK_IDENTIFIER);
    t.val = strndup(l->src + start, length);

    static Keyword keywords[] = {
        {"return", TOK_KW_RETURN},
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
