#ifndef LEXER_H
#define LEXER_H
#include <stdint.h>
#include <sys/types.h>

typedef enum {
  TOK_EOF,
  TOK_ERROR,

  TOK_SEMICOLON,
  TOK_COLON,

  TOK_LPARAN,
  TOK_RPARAN,
  TOK_LBRACE,
  TOK_RBRACE,
  TOK_IDENTIFIER,

  TOK_STR_LIT,
  TOK_INT_LIT,

  TOK_KW_RETURN,
  TOK_KW_FUNC,
} TokenKind;

typedef struct {
  char *file_name;

  char *val;
  TokenKind kind;

  int line;
  int col;
} Token;

typedef struct {
  const char *word;
  TokenKind kind;
} Keyword;

typedef struct {
  char *src;
  char *file;
  int pos;
  int line;
  int col;
} Lexer;

char *kind_str(TokenKind kind);
Lexer lexer_init(char *file, char *src);

Token next_token(Lexer *l);

#endif
