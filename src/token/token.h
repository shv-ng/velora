#pragma once

enum TokenKind {
  TOK_EOF,
  TOK_ERROR,

  TOK_SEMICOLON, // ;
  TOK_COLON,     // :

  TOK_LPAREN, // (
  TOK_RPAREN, // )
  TOK_LBRACE, // {
  TOK_RBRACE, // }

  TOK_IDENTIFIER,

  TOK_STR_LITERAL, // string
  TOK_INT_LITERAL, // int cont

  TOK_KW_RETURN, // "return"
  TOK_KW_FUNC,   // "fn"

  TOK_PLUS,        // +
  TOK_MINUS,       // -
  TOK_STAR,        // *
  TOK_SLASH,       // /
  TOK_MODULO,      // %

  // bitwise
  TOK_AMPERSAND,   // &
  TOK_PIPE,        // |
  TOK_CARET,       // ^
  TOK_TIDLE,       // ~
  TOK_LEFT_SHIFT,  // <<
  TOK_RIGHT_SHIFT, // >>

  TOK_BANG,        // !
  TOK_LEFT_ARROW,  // <
  TOK_RIGHT_ARROW, // >
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
