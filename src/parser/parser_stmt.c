#include "parser_internal.h"
#include <stddef.h>

struct AstNode *parse_return_stmt(struct Parser *p) {
  struct Span start = p->current_token.span;
  expect(p, TOK_KW_RETURN);

  struct AstNode *return_stmt = astnode_new(p, AST_RETURN_STMT);

  if (p->current_token.kind != TOK_SEMICOLON) {
    return_stmt->as.return_stmt.expr = parse_expr(p, 0);
  }

  struct Span end = p->current_token.span;
  expect(p, TOK_SEMICOLON);
  return_stmt->span = merge_span(start, end);

  return return_stmt;
}

struct AstNode *parse_block(struct Parser *p, char *name) {
  struct Span start = p->current_token.span;

  expect(p, TOK_LBRACE);

  struct AstNode *block = astnode_new(p, AST_BLOCK_DECL);
  block->as.block.trailing_expr = NULL;
  block->as.block.name = name;

  size_t capacity = 10;
  size_t count = 0;

  struct AstNode **statements =
      arena_malloc(p->arena, sizeof(struct AstNode *) * capacity);

  while (p->current_token.kind != TOK_EOF &&
         p->current_token.kind != TOK_RBRACE) {
    struct AstNode *stmt = NULL;

    switch (p->current_token.kind) {
    case TOK_KW_RETURN:
      stmt = parse_return_stmt(p);
      break;
    default: {
      // try it as expr first
      struct AstNode *expr = parse_expr(p, 0);
      // it's statement
      if (p->current_token.kind == TOK_SEMICOLON) {
        advance(p);
        stmt = astnode_new(p, AST_EXPR_STMT);
        stmt->as.expr_stmt.expr = expr;
      } else if (p->current_token.kind == TOK_RBRACE) {
        block->as.block.trailing_expr = expr;
      } else {
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
    }
    if (stmt != NULL) {
      da_append(p->arena, (void ***)&statements, (void *)stmt, &count,
                &capacity);
    }
  }

  block->as.block.statements = statements;
  block->as.block.count = count;

  struct Span end = p->current_token.span;
  expect(p, TOK_RBRACE);

  block->span = merge_span(start, end);

  return block;
}
