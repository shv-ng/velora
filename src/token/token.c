#include "token.h"

char *token_kind_str(enum TokenKind kind) {
  switch (kind) {
  case TOK_EOF:
    return "end of file";
  case TOK_SEMICOLON:
    return "';'";
  case TOK_ERROR:
    return "unknown token";
  case TOK_COLON:
    return "':'";
  case TOK_LPAREN:
    return "'('";
  case TOK_RPAREN:
    return "')'";
  case TOK_LBRACE:
    return "'{'";
  case TOK_RBRACE:
    return "'}'";
  case TOK_STR_LITERAL:
    return "str literal";
  case TOK_IDENTIFIER:
    return "identifier";
  case TOK_INT_LITERAL:
    return "int literal";
  case TOK_KW_RETURN:
    return "'return' keyword";
  case TOK_KW_FUNC:
    return "'fn' Keyword";
  }
}

struct Span merge_span(struct Span s1, struct Span s2) {
  struct Span span;
  span.start_col = s1.start_col < s2.start_col ? s1.start_col : s2.start_col;
  span.end_col = s1.end_col > s2.end_col ? s1.end_col : s2.end_col;

  span.start_line =
      s1.start_line < s2.start_line ? s1.start_line : s2.start_line;
  span.end_line = s1.end_line > s2.end_line ? s1.end_line : s2.end_line;

  return span;
}
