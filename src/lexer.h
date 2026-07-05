#ifndef LEXER_H
#define LEXER_H
#include <stdint.h>
#include <sys/types.h>

enum TokenKind {
  TOK_EOF,
  TOK_ERROR,

  TOK_SEMICOLON,
  TOK_COLON,

  TOK_LPARAN,
  TOK_RPARAN,
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

struct Lexer {
  char *src;
  char *file;
  int pos;
  struct Span current_span;
};

char *kind_str(enum TokenKind kind);
inline struct Span merge_span(struct Span s1, struct Span s2);
struct Lexer lexer_init(char *file, char *src);

struct Token next_token(struct Lexer *l);

#endif
