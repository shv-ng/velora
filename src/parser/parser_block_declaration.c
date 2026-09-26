#include "parser_internal.h"
#include <stddef.h>
#include "../utils/da.h"

struct AstNode *parse_block_declaration(struct ParserCtx *ctx, char *name) {
  struct Span start = ctx->current_token.span;

  parser_expect(ctx, TOK_LBRACE);

  struct AstNode *block = astnode_new(ctx, AST_BLOCK_DECLARATION);
  block->as.block.trailing_expression = NULL;
  block->as.block.name = name;

  size_t capacity = 10;
  size_t count = 0;

  struct AstNode **statements =
      arena_malloc(ctx->arena, sizeof(struct AstNode *) * capacity);

  while (ctx->current_token.kind != TOK_EOF &&
         ctx->current_token.kind != TOK_RBRACE) {

    struct AstNode *stmt = parse_statement(ctx, block);

    if (stmt != NULL) {
      da_append(ctx->arena, (void ***)&statements, (void *)stmt, &count,
                &capacity);
    }
  }

  block->as.block.statements = statements;
  block->as.block.count = count;

  struct Span end = ctx->current_token.span;
  parser_expect(ctx, TOK_RBRACE);

  block->span = merge_span(start, end);

  return block;
}
