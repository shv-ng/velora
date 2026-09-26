#include "parser_internal.h"

struct AstNode *parse_variable_declaration(struct ParserCtx *ctx, char *name,
                               struct AstNode *type, struct Span start) {

  parser_expect(ctx, TOK_EQUAL);

  struct AstNode *node = astnode_new(ctx, AST_VARIABLE_DECLARATION);
  node->as.variable_declaration.name = name;
  node->as.variable_declaration.type = type;
  node->as.variable_declaration.expression = parse_expression(ctx, 0);

  struct Span end = ctx->current_token.span;

  parser_expect(ctx, TOK_SEMICOLON);
  node->span = merge_span(start, end);

  return node;
}
