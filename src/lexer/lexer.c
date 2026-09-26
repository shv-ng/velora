#include "lexer_internal.h"
#include <ctype.h>

// create new lexer ctx, contain ptr of contents and file_name
struct LexerCtx lexer_new(struct Arena *a, char *file_name, char *contents) {
  return (struct LexerCtx){
      .arena = a,
      .file_name = file_name,
      .contents = contents,
      .pos = 0,
      .span = (struct Span){
          .start_col = 1, .end_col = 1, .start_line = 1, .end_line = 1}};
}

// it'll move forward and return a token at a time.
struct Token lexer_next_token(struct LexerCtx *ctx) {
  lexer_skip_whitespace(ctx);

  // new token start when whitespace get skipped and old token ends
  ctx->span.start_col = ctx->span.end_col;
  ctx->span.start_line = ctx->span.end_line;

  char c = lexer_peek(ctx);

  switch (c) {
  case '\0':
    return lexer_make_tok(ctx, TOK_EOF);
  case ';':
    lexer_advance(ctx);
    return lexer_make_tok(ctx, TOK_SEMICOLON);
  case ':':
    lexer_advance(ctx);
    return lexer_make_tok(ctx, TOK_COLON);
  case '(':
    lexer_advance(ctx);
    return lexer_make_tok(ctx, TOK_LPAREN);
  case ')':
    lexer_advance(ctx);
    return lexer_make_tok(ctx, TOK_RPAREN);
  case '{':
    lexer_advance(ctx);
    return lexer_make_tok(ctx, TOK_LBRACE);
  case '}':
    lexer_advance(ctx);
    return lexer_make_tok(ctx, TOK_RBRACE);
  case '+':
    lexer_advance(ctx);
    return lexer_make_tok(ctx, TOK_PLUS);
  case '-':
    lexer_advance(ctx);
    return lexer_make_tok(ctx, TOK_MINUS);
  case '*':
    lexer_advance(ctx);
    return lexer_make_tok(ctx, TOK_STAR);
  case '/':
    lexer_advance(ctx);
    return lexer_make_tok(ctx, TOK_SLASH);
  case '%':
    lexer_advance(ctx);
    return lexer_make_tok(ctx, TOK_MODULO);
  case '&':
    lexer_advance(ctx);
    return lexer_make_tok(ctx, TOK_AMPERSAND);
  case '|':
    lexer_advance(ctx);
    return lexer_make_tok(ctx, TOK_PIPE);
  case '^':
    lexer_advance(ctx);
    return lexer_make_tok(ctx, TOK_CARET);
  case '~':
    lexer_advance(ctx);
    return lexer_make_tok(ctx, TOK_TIDLE);
  case '!':
    lexer_advance(ctx);
    return lexer_make_tok(ctx, TOK_BANG);
  case '=':
    lexer_advance(ctx);
    return lexer_make_tok(ctx, TOK_EQUAL);
  case '<': {
    lexer_advance(ctx);
    c = lexer_peek(ctx);
    if (c == '<') {
      lexer_advance(ctx);
      return lexer_make_tok(ctx, TOK_LEFT_SHIFT);
    }
    return lexer_make_tok(ctx, TOK_LEFT_ARROW);
  }
  case '>': {
    lexer_advance(ctx);
    c = lexer_peek(ctx);
    if (c == '>') {
      lexer_advance(ctx);
      return lexer_make_tok(ctx, TOK_RIGHT_SHIFT);
    }
    return lexer_make_tok(ctx, TOK_RIGHT_ARROW);
  }
  }

  if (isdigit(c)) {
    return lexer_number(ctx);
  }

  if (isalpha(c) || c == '_') {
    return lexer_identifier(ctx); // it'll also handle keywords
  }

  lexer_advance(ctx);
  return lexer_make_tok(ctx, TOK_ERROR);
}
