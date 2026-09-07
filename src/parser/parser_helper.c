#include "parser_internal.h"

struct AstNode *astnode_new(struct Parser *p, enum AstKind kind) {
  struct AstNode *node = arena_calloc(p->arena, 1, sizeof(struct AstNode));
  node->resolved_type = &type_unknown;
  node->kind = kind;
  return node;
}

void parser_advance(struct Parser *p) {
  p->current_token = p->next_token;
  p->next_token = next_token(p->lexer);
}

void synchronise(struct Parser *p) {
  // dumber error recovery
  while (p->current_token.kind != TOK_EOF) {
    switch (p->current_token.kind) {
    case TOK_SEMICOLON:
      parser_advance(p);
      return;

    case TOK_EOF:
    case TOK_LBRACE:
    case TOK_RBRACE:
      return;

    default:
      parser_advance(p);
    }
  }
}

void expect(struct Parser *p, enum TokenKind kind) {
  if (p->current_token.kind == kind) {
    parser_advance(p);
    return;
  }

  struct Error err = {
      .kind = ERR_SYNTAX,
      .span = p->current_token.span,
      .as.syntax.expected = token_kind_str(kind),
      .as.syntax.found = token_kind_str(p->current_token.kind),
  };
  print_error(err, p->lexer->file_name, p->lexer->contents);

  p->error_count++;
  return;
}
