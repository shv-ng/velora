#include "parser_internal.h"
#include <stdlib.h>

struct AstNode *parse_primary(struct Parser *p) {

  if (p->current_token.kind == TOK_INT_LITERAL) {
    struct Token int_tok = p->current_token;
    expect(p, TOK_INT_LITERAL);

    struct AstNode *node = astnode_new(p, AST_INT_LITERAL);
    node->as.int_literal.value = atoll(int_tok.val);
    node->span = int_tok.span;

    return node;
  }

  if (p->current_token.kind == TOK_LPAREN) {
    advance(p);

    struct AstNode *expr = parse_expr(p, 0);

    expect(p, TOK_RPAREN);
    return expr;
  }
  struct Error err = {
      .kind = ERR_SYNTAX,
      .span = p->current_token.span,
      .as.syntax.expected = "expression",
      .as.syntax.found = token_kind_str(p->current_token.kind),
  };
  print_error(err, p->lexer->file_name, p->lexer->contents);
  p->error_count++;
  synchronise(p);
  return NULL;
}
