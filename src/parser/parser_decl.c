#include "parser.h"
#include "parser_internal.h"

struct AstNode *parse_declaration(struct Parser *p) {
  struct Token name_tok = p->current_token;

  expect(p, TOK_IDENTIFIER);
  expect(p, TOK_COLON);

  switch (p->current_token.kind) {
  case TOK_KW_FUNC:
    return parse_func_decl(p, name_tok.val);
  case TOK_IDENTIFIER: {
    struct AstNode *type = parse_type(p);
    if (p->current_token.kind == TOK_EQUAL) {
      return parse_var_decl(p, name_tok.val, type);
    }
  }

  default: {
    struct Error err = {
        .kind = ERR_SYNTAX,
        .span = p->current_token.span,
        .as.syntax.found = token_kind_str(p->current_token.kind),
    };

    print_error(err, p->lexer->file_name, p->lexer->contents);
    p->error_count++;

    parser_advance(p);
    return NULL;
  }
  }
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

struct AstNode *parse_var_decl(struct Parser *p, char *name,
                               struct AstNode *type) {

  expect(p, TOK_EQUAL);

  struct AstNode *node = astnode_new(p, AST_VAR_DECL);
  node->as.var_decl.name = name;
  node->as.var_decl.type = type;
  node->as.var_decl.expr = parse_expr(p, 0);
  expect(p, TOK_SEMICOLON);

  return node;
}
