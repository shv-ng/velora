#pragma once

enum TokenKind {
  TOK_EOF,
  TOK_ERROR,

  TOK_SEMICOLON,
  TOK_COLON,

  TOK_LPAREN,
  TOK_RPAREN,

  TOK_LBRACE,
  TOK_RBRACE,
  TOK_IDENTIFIER,

  TOK_STR_LITERAL,
  TOK_INT_LITERAL,

  TOK_KW_RETURN,
  TOK_KW_FUNC,
};

struct Span {
  int start_line;
  int end_line;
  int start_col;
  int end_col;
};

struct Token {
  struct Span span;

  char *file_name;
  char *val;
  enum TokenKind kind;
};

struct Keyword {
  const char *word;
  enum TokenKind kind;
};

char *token_kind_str(enum TokenKind kind);
struct Span merge_span(struct Span s1, struct Span s2);
