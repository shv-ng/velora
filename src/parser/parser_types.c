#include "parser_internal.h"

struct AstNode *parse_type(struct Parser *p) {
  struct Span start = p->current_token.span;
  struct AstNode *type = astnode_new(p, AST_TYPE_UNKNOWN);

  if (p->current_token.kind == TOK_IDENTIFIER) {
    struct Token name_tok = p->current_token;
    expect(p, TOK_IDENTIFIER);

    type->kind = AST_TYPE_NAMED;
    type->as.type_named.name = name_tok.val;
  }

  if (type->kind == AST_TYPE_UNKNOWN) {
    struct Error err = {
        .kind = ERR_SYNTAX,
        .span = p->current_token.span,
        .as.syntax.found = token_kind_str(p->current_token.kind),
    };
    print_error(err, p->lexer->file_name, p->lexer->contents);
    advance(p);
    p->error_count++;
  }
  struct Span end = p->current_token.span;
  type->span = merge_span(start, end);
  return type;
}
