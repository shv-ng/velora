#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>

static const char *get_line_ptr(const char *contents, int target_line) {
  int current_line = 1;
  const char *ptr = contents;

  while (current_line < target_line && *ptr != '\0') {
    if (*ptr == '\n' || *ptr == '\r') {
      current_line++;
      if (*ptr == '\r' && *(ptr + 1) == '\n') {
        ptr++;
      }
    }
    ptr++;
  }
  return ptr;
}

static int get_line_len(const char *line_ptr) {
  int len = 0;
  while (line_ptr[len] != '\0' && line_ptr[len] != '\r' &&
         line_ptr[len] != '\n') {
    len++;
  }
  return len;
}

static void print_error_body(const char *file_name, const char *contents,
                             struct Span span) {

  fprintf(stderr, "%s ->%s ", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);

  fprintf(stderr, "%s:%d:%d\n", file_name, span.start_line, span.start_col);

  const char *start_ptr = get_line_ptr(contents, span.start_line);
  const int start_len = get_line_len(start_ptr);

  fprintf(stderr, " %s%4d |%s %.*s\n", ANSI_COLOR_BLUE, span.start_line,
          ANSI_COLOR_RESET, start_len, start_ptr);
  fprintf(stderr, "      %s|%s ", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);

  for (int i = 1; i < span.start_col; i++) {
    fprintf(stderr, " ");
  }

  if (span.start_line == span.end_line) {
    int span_len = ((span.end_col - span.start_col) <= 0)
                       ? 1
                       : (span.end_col - span.start_col);

    fprintf(stderr, "%s", ANSI_COLOR_RED);
    for (int i = 0; i < span_len; i++) {
      fprintf(stderr, "^");
    }
    fprintf(stderr, "%s", ANSI_COLOR_RESET);
  } else {
    fprintf(stderr, "%s^%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);

    if (span.end_line - span.start_line > 1) {
      fprintf(stderr, "  ... %s|%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    }

    const char *end_ptr = get_line_ptr(contents, span.end_line);
    const int end_len = get_line_len(end_ptr);

    fprintf(stderr, " %s%4d |%s %.*s\n", ANSI_COLOR_BLUE, span.end_line,
            ANSI_COLOR_RESET, end_len, end_ptr);

    fprintf(stderr, "      %s|%s ", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    for (int i = 1; i < span.end_col; i++) {
      fprintf(stderr, " ");
    }
    fprintf(stderr, "%s^%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
  }
  fprintf(stderr, "\n");
}

static const char *error_fmt(enum ErrorKind kind) {
  switch (kind) {
  case ERR_EXPECTED_FOUND:
    return "expected '%s', found '%s'";
  case ERR_UNEXPECTED:
    return "unexpected '%s'";
  case ERR_TYPE_MISMATCH:
    return "type mismatch: expected '%s', found '%s'";
  case ERR_TYPE_MISMATCH_CONTEXT:
    return "type mismatch: expected '%s', found '%s' in %s";
  case ERR_MISSING_RETURN:
    return "function '%s' must return '%s' but has no return statement";
  case ERR_CODEGEN:
    return "codegen: %s";
  case ERR_MEMORY:
    return "memory: %s";
  case ERR_UNDEFINED_IDENTIFIER:
    return "cannot find '%s' in this scope";
  case ERR_UNUSED_IDENTIFIER:
    return "'%s' is defined but never used";
  case ERR_REDECLARATION:
    return "'%s' already defined in this scope";
  case ERR_INVALID_LVALUE:
    return "expression is not assignable";
  case ERR_UNRESOLVED_TYPE:
    return "ICE: unresolved types before codegen";
  case ERR_FILE:
    return "file error: %s";
  case ERR_OVERFLOW:
    return "value '%s' overflows type '%s'";
  }
}

void emit_error(struct ErrorCtx *err, struct Span span, enum ErrorKind kind,
                ...) {
  if (err) {
    err->count++;
  }

  const char *fmt = error_fmt(kind);

  // error header
  fprintf(stderr, "%serror%s: %s", ANSI_COLOR_RED, ANSI_COLOR_RESET,
          ANSI_COLOR_BOLD);

  // take the variable len of args i.e. '...' after 'kind'
  va_list args;
  va_start(args, kind);
  vfprintf(stderr, fmt, args);
  va_end(args);

  fprintf(stderr, "%s\n", ANSI_COLOR_RESET);

  if (err && span.start_line != 0) {
    print_error_body(err->file_name, err->content, span);
  }
}
