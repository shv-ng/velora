#ifndef ERROR_H
#define ERROR_H

#include "lexer.h"

#define ANSI_COLOR_RED "\033[1;31m"
#define ANSI_COLOR_BLUE "\033[1;34m"
#define ANSI_COLOR_BOLD "\033[1m"
#define ANSI_COLOR_RESET "\033[0m"

enum ErrorKind {
  ERR_SYNTAX,
  ERR_TYPE_MISMATCH,
};

struct ErrTypeMismatch {
  const char *expected;
  const char *found;
  const char *context;
};

struct ErrSyntax {
  const char *expected;
  const char *found;
};

struct Error {
  enum ErrorKind kind;
  struct Span span;

  union {
    struct ErrSyntax syntax;
    struct ErrTypeMismatch type_mismatch;
  } as;
};

void print_error(struct Error error, const char *file_name,
                 const char *contents);

#endif
