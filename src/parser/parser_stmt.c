#include "parser_internal.h"

struct AstNode *parse_return_stmt(struct Parser *p) {
  struct Span start = p->current_token.span;
  expect(p, TOK_KW_RETURN);

  struct AstNode *return_stmt = astnode_new(p, AST_RETURN_STMT);

  if (p->current_token.kind != TOK_SEMICOLON) {
    return_stmt->as.return_stmt.expr = parse_expr(p);
  }

  struct Span end = p->current_token.span;
  expect(p, TOK_SEMICOLON);
  return_stmt->span = merge_span(start, end);

  return return_stmt;
}

struct AstNode *parse_block(struct Parser *p, char *name) {
  struct Span start = p->current_token.span;
  expect(p, TOK_LBRACE);

  int capacity = 10;
  struct AstNode **statements =
      arena_malloc(p->arena, sizeof(struct AstNode *) * capacity);
  int count = 0;

  while (p->current_token.kind != TOK_EOF &&
         p->current_token.kind != TOK_RBRACE) {
    if (count >= capacity) {
      size_t old_size = capacity * sizeof(struct AstNode *);

      capacity *= 2;
      size_t new_size = capacity * sizeof(struct AstNode *);

      statements = arena_realloc(p->arena, statements, old_size, new_size);
    }
    struct AstNode *stmt = NULL;

    switch (p->current_token.kind) {
    case TOK_KW_RETURN:
      stmt = parse_return_stmt(p);
      break;
    default: {
      struct Error err = {
          .kind = ERR_SYNTAX,
          .span = p->current_token.span,
          .as.syntax.found = token_kind_str(p->current_token.kind),
      };
      print_error(err, p->lexer->file_name, p->lexer->contents);
      p->error_count++;

      synchronise(p);
      continue;
    }
    }
    if (stmt != NULL) {
      statements[count++] = stmt;
    }
  }

  struct AstNode *block = astnode_new(p, AST_BLOCK_DECL);

  block->as.block.name = name;
  block->as.block.statements = statements;
  block->as.block.count = count;

  expect(p, TOK_RBRACE);
  struct Span end = p->current_token.span;
  block->span = merge_span(start, end);
  return block;
}
