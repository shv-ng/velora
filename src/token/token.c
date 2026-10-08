#include "token.h"
#include <stdbool.h>

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

  case TOK_KW_RETURN:
    return "'return' keyword";
  case TOK_KW_FUNC:
    return "'fn' keyword";
  case TOK_KW_IF:
    return "'if' keyword";
  case TOK_KW_ELSE:
    return "'else' keyword";

  case TOK_PLUS:
    return "'+' symbol";
  case TOK_MINUS:
    return "'-' symbol";
  case TOK_STAR:
    return "'*' symbol";
  case TOK_SLASH:
    return "'/' symbol";
  case TOK_MODULO:
    return "'%' symbol";
  case TOK_AMPERSAND:
    return "'&' symbol";
  case TOK_PIPE:
    return "'|' symbol";
  case TOK_CARET:
    return "'^' symbol";
  case TOK_TIDLE:
    return "'~' symbol";
  case TOK_LEFT_SHIFT:
    return "'<<' symbol";
  case TOK_RIGHT_SHIFT:
    return "'>>' symbol";
  case TOK_BANG:
    return "'!' symbol";
  case TOK_LEFT_ARROW:
    return "'<' symbol";
  case TOK_RIGHT_ARROW:
    return "'>' symbol";
  case TOK_EQUAL:
    return "'=' symbol";
  case TOK_TRUE:
    return "'true' literal";
  case TOK_FALSE:
    return "'false' literal";
  case TOK_STR_LITERAL:
    return "str literal";
  case TOK_IDENTIFIER:
    return "identifier";
  case TOK_INT_LITERAL:
    return "int literal";
  case TOK_COMMENT:
    return "comment";
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

bool comment_attacted(struct Token comment, struct Token next_token) {
  return comment.kind == TOK_COMMENT &&
         next_token.span.start_line - comment.span.end_line == 1;
}
