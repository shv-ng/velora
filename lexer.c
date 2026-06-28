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
  char c = l->src[l->pos++];
  if (c == '\n') {
    l->line++;
    l->col = 1;
  } else {
    l->col++;
  }
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
    long long val = 0;
    while (isdigit(peek(l))) {
      val = val * 10 + (advance(l) - '0');
    }
    Token t = make_tok(l, TOK_INT_LIT);
    t.val.int_val = val;
    return t;
  }

  if (isalpha(c) || c == '_') {
    char buf[128];
    int i = 0;
    while (isalpha(peek(l)) || peek(l) == '_') {
      buf[i++] = advance(l);
    }
    buf[i] = '\0';

    static Keyword keywords[] = {
        {"return", TOK_KW_RETURN},
        {NULL, 0},
    };

    for (int k = 0; keywords[k].word; k++) {
      if (strcmp(buf, keywords[k].word) == 0)
        return make_tok(l, keywords[k].kind);
    }

    Token t = make_tok(l, TOK_STR_LIT);
    t.val.str_val = buf;
    return t;
  }

  advance(l);
  return make_tok(l, TOK_ERROR);
}
