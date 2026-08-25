#include "parser_internal.h"

struct AstNode *parse_declaration(struct Parser *p) {
  struct Token name_tok = p->current_token;
  expect(p, TOK_IDENTIFIER);
  expect(p, TOK_COLON);

  if (p->current_token.kind == TOK_KW_FUNC) {
    return parse_func_decl(p, name_tok.val);
  }
  return NULL;
}

struct AstNode *parse_func_decl(struct Parser *p, char *name) {
  struct Span start = p->current_token.span;
  expect(p, TOK_KW_FUNC);

  expect(p, TOK_LPAREN);
  expect(p, TOK_RPAREN);

  struct AstNode *return_type = parse_type(p);
  struct AstNode *block = parse_block(p, name);

  if (p->error_count > 0) {
    return NULL;
  }

  struct AstNode *func = astnode_new(p, AST_FUNCTION_DECL);

  func->as.function.name = name;
  func->as.function.return_type = return_type;
  func->as.function.block = block;

  struct Span end = p->current_token.span;
  func->span = merge_span(start, end);
  return func;
}
