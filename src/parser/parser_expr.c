#include "parser_internal.h"
#include <stdlib.h>

struct AstNode *parse_expr(struct Parser *p) {
  switch (p->current_token.kind) {
  case TOK_INT_LITERAL: {

    struct Token int_tok = p->current_token;
    expect(p, TOK_INT_LITERAL);

    struct AstNode *node = astnode_new(p, AST_INT_LITERAL);
    node->as.int_literal.value = atoll(int_tok.val);
    node->span = int_tok.span;

    return node;
  }
  default: {
    struct Error err = {
        .kind = ERR_SYNTAX,
        .span = p->current_token.span,
        .as.syntax.expected = "expression",
        .as.syntax.found = token_kind_str(p->current_token.kind),
    };

    print_error(err, p->lexer->file_name, p->lexer->contents);
    synchronise(p);
    p->error_count++;
    return NULL;
  }
  }
}
