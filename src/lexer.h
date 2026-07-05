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

struct Token {
  char *file_name;

  char *val;
  enum TokenKind kind;

  int line;
  int col;
};

struct Keyword {
  const char *word;
  enum TokenKind kind;
};

struct Lexer {
  char *src;
  char *file;
  int pos;
  int line;
  int col;
};

char *kind_str(enum TokenKind kind);
struct Lexer lexer_init(char *file, char *src);

struct Token next_token(struct Lexer *l);

#endif
