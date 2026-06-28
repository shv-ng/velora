#ifndef LEXER_H
#define LEXER_H
#include <stdint.h>
#include <sys/types.h>
#define TOKEN_KINDS                                                            \
  X(TOK_EOF)                                                                   \
  X(TOK_ERROR)                                                                 \
  X(TOK_COLON)                                                                 \
  X(TOK_LPARAN)                                                                \
  X(TOK_RPARAN)                                                                \
  X(TOK_LBRACE)                                                                \
  X(TOK_RBRACE)                                                                \
  X(TOK_STR_LIT)                                                               \
  X(TOK_INT_LIT)                                                               \
  X(TOK_KW_RETURN)

typedef enum {
#define X(name) name,
  TOKEN_KINDS
#undef X
} TokenKind;

typedef union {
  long long int_val;
  char *str_val;
} TokenVal;

typedef struct {
  char *file_name;

  TokenVal val;
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
