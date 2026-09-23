#pragma once

#include "../token/token.h"

#define ANSI_COLOR_RED "\033[1;31m"
#define ANSI_COLOR_BLUE "\033[1;34m"
#define ANSI_COLOR_BOLD "\033[1m"
#define ANSI_COLOR_RESET "\033[0m"

enum ErrorKind {
  ERR_SYNTAX,
  ERR_TYPE_MISMATCH,
  ERR_MISSING_RETURN,
  ERR_UNDEFINED_IDENTIFIER,
  ERR_UNUSED_IDENTIFIER,
  ERR_REDECLARATION,
  ERR_CODEGEN,
  ERR_MEMORY,
};

struct ErrRedeclaration {
  const char *name;
};

struct ErrUnusedIdentifier {
  const char *name;
};

struct ErrUndefinedIdentifier {
  const char *name;
};

struct ErrMemory {
  const char *message;
};

struct ErrCodegen {
  const char *message;
};

struct ErrMissingReturn {
  const char *expected;
  const char *fn_name;
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
    struct ErrMissingReturn missing_return;
    struct ErrUndefinedIdentifier undefined_identifier;
    struct ErrUnusedIdentifier unused_identifier;
    struct ErrRedeclaration redeclaration;
  } as;
};

void print_error(struct Error error, const char *file_name,
                 const char *contents);
