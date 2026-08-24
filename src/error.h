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
  ERR_CODEGEN,
  ERR_MEMORY,
};

struct ErrMemory {
  const char *message;
};

struct ErrCodegen {
  const char *message;
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
    struct ErrCodegen codegen;
    struct ErrMemory memory;
  } as;
};

void print_error(struct Error error, const char *file_name,
                 const char *contents);

#endif
