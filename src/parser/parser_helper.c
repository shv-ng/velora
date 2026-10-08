#include "parser.h"
#include "parser_internal.h"
#include <stddef.h>
#include <string.h>

const int max_err_limit = 10;

struct AstNode *astnode_new(struct ParserCtx *ctx, enum AstKind kind) {
  struct AstNode *node = arena_calloc(ctx->arena, 1, sizeof(struct AstNode));
  node->resolved_type = &type_unknown;
  node->kind = kind;
  return node;
}

void parser_advance(struct ParserCtx *ctx) {
  ctx->current_token = ctx->next_token;
  ctx->next_token = lexer_next_token(ctx->lexer);
}

void parser_synchronise(struct ParserCtx *ctx) {

  // dumber error recovery
  while (ctx->current_token.kind != TOK_EOF) {
    if (ctx->err->count > max_err_limit) {
      ctx->current_token.kind = TOK_EOF;
    };

    switch (ctx->current_token.kind) {
    case TOK_SEMICOLON:
      parser_advance(ctx);
      return;

    case TOK_EOF:
    case TOK_LBRACE:
    case TOK_RBRACE:
      return;

    default:
      parser_advance(ctx);
    }
  }
}

void parser_expect(struct ParserCtx *ctx, enum TokenKind kind) {
  if (ctx->current_token.kind == kind) {
    parser_advance(ctx);
    return;
  }

  emit_error(ctx->err, ctx->current_token.span, ERR_EXPECTED_FOUND,
             token_kind_str(kind), token_kind_str(ctx->current_token.kind));
  return;
}

struct Token comment_merge(struct ParserCtx *ctx, struct Token comment1,
                           struct Token comment2) {
  size_t m = strlen(comment1.val);
  size_t n = strlen(comment2.val);
  size_t size = m + n + 2; // 1 + 1 (\0 + \n)

  char *new_comment = arena_malloc(ctx->arena, size);
  for (size_t i = 0; i < size; i++) {
    if (i < m) {
      new_comment[i] = comment1.val[i];
    } else if (i == m) {
      new_comment[i] = '\n';
    } else {
      new_comment[i] = comment2.val[i - m - 1];
    }
  }

  comment1.val = new_comment;
  comment1.span = merge_span(comment1.span, comment2.span);

  return comment1;
}
