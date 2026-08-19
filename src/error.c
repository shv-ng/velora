#include "error.h"
#include "lexer.h"
#include <stdio.h>

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

void print_error(struct Error error, const char *file_name,
                 const char *contents) {
  // error header
  fprintf(stderr, "%serror:%s ", ANSI_COLOR_RED, ANSI_COLOR_RESET);

  switch (error.kind) {
  case ERR_SYNTAX:
    if (error.as.syntax.expected) {
      fprintf(stderr, "%sexpected %s, found %s%s\n", ANSI_COLOR_BOLD,
              error.as.syntax.expected, error.as.syntax.found,
              ANSI_COLOR_RESET);
    } else {
      fprintf(stderr, "%sunexpected %s%s\n", ANSI_COLOR_BOLD,
              error.as.syntax.found, ANSI_COLOR_RESET);
    }
    break;
  case ERR_TYPE_MISMATCH:
    fprintf(stderr, "%stype mismatch in %s: expected %s, found %s%s\n",
            ANSI_COLOR_BOLD, error.as.type_mismatch.context,
            error.as.type_mismatch.expected, error.as.type_mismatch.found,
            ANSI_COLOR_RESET);
    break;
  case ERR_CODEGEN:
    fprintf(stderr, "%scodegen error: %s%s\n", ANSI_COLOR_BOLD,
            error.as.codegen.message, ANSI_COLOR_RESET);
    break;
  }

  print_error_body(file_name, contents, error.span);
}
