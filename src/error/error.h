#pragma once

#include "../token/token.h"

#define ANSI_COLOR_RED "\033[1;31m"
#define ANSI_COLOR_BLUE "\033[1;34m"
#define ANSI_COLOR_BOLD "\033[1m"
#define ANSI_COLOR_RESET "\033[0m"

// the num represent how much variable needed
enum ErrorKind {
  // 0: expected value;  1: what found
  ERR_EXPECTED_FOUND,
  // 0: what found
  ERR_UNEXPECTED,
  // 0: expected type;  1: what type found
  ERR_TYPE_MISMATCH,
  // 0: expected type;  1: what type found; 2: context
  ERR_TYPE_MISMATCH_CONTEXT,
  // 0: fn name; 1: return type
  ERR_MISSING_RETURN,
  // 0: identifier name
  ERR_UNDEFINED_IDENTIFIER,
  // 0: msg
  ERR_CODEGEN,
  // 0: identifier name
  ERR_UNUSED_IDENTIFIER,
  // 0: identifier name
  ERR_REDECLARATION,
  // nothing neeed
  ERR_INVALID_LVALUE,
  // nothing neeed
  ERR_UNRESOLVED_TYPE,
  // 0: msg
  ERR_MEMORY,
  // 0: msg
  ERR_FILE,
  // 0: value; 1: type
  ERR_OVERFLOW,
};

struct ErrorCtx {
  int count;

  const char *file_name;
  const char *content;
};

void emit_error(struct ErrorCtx *err, struct Span span, enum ErrorKind kind,
                ...);
