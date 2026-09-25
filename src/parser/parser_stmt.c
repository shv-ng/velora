#include "parser.h"
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

bool is_valid_lvalue(struct AstNode *lhs) {
  switch (lhs->kind) {
  case AST_IDENTIFIER:
    return true;
  default:
    return false;
  }
}

struct AstNode *parse_stmt(struct Parser *p, struct AstNode *block) {
  if (p->current_token.kind == TOK_KW_RETURN) {
    return parse_return_stmt(p);
  }
  if (p->current_token.kind == TOK_IDENTIFIER) {
    if (p->next_token.kind == TOK_COLON) {
      return parse_declaration(p);
    }
  }

  struct AstNode *expr = parse_expr(p, 0);

  // it's statement
  if (p->current_token.kind == TOK_SEMICOLON) {
    parser_advance(p);

    struct AstNode *stmt = astnode_new(p, AST_EXPR_STMT);
    stmt->as.expr_stmt.expr = expr;

    return stmt;
  }

  // it's a trailing_expr
  if (p->current_token.kind == TOK_RBRACE) {
    block->as.block.trailing_expr = expr;
    return NULL;
  }

  if (p->current_token.kind == TOK_EQUAL) {
    if (!is_valid_lvalue(expr)) {
      struct Error err = {
          .kind = ERR_INVALID_LVALUE,
          .span = expr->span,
      };

      print_error(err, p->lexer->file_name, p->lexer->contents);
      p->error_count++;
      synchronise(p);

      return NULL;
    }

    expect(p, TOK_EQUAL);
    struct AstNode *rhs = parse_expr(p, 0);

    struct Span end = p->current_token.span;
    expect(p, TOK_SEMICOLON);

    struct AstNode *stmt = astnode_new(p, AST_ASSIGNMENT);

    stmt->as.assignment.lhs = expr;
    stmt->as.assignment.rhs = rhs;
    stmt->span = merge_span(expr->span, end);

    return stmt;
  }

  struct Error err = {
      .kind = ERR_SYNTAX,
      .span = p->current_token.span,
      .as.syntax.found = token_kind_str(p->current_token.kind),
  };
  print_error(err, p->lexer->file_name, p->lexer->contents);
  p->error_count++;

  synchronise(p);
  return NULL;
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

    struct AstNode *stmt = parse_stmt(p, block);

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
