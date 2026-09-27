#include "parser_internal.h"

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
