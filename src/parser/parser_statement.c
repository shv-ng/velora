#include "parser_internal.h"
#include <stddef.h>

bool parser_is_valid_lvalue(struct AstNode *lhs) {
  switch (lhs->kind) {
  case AST_IDENTIFIER:
    return true;
  default:
    return false;
  }
}

struct AstNode *parse_statement(struct ParserCtx *ctx, struct AstNode *block) {
  if (ctx->current_token.kind == TOK_KW_RETURN) {
    return parse_return_statement(ctx);
  }
  if (ctx->current_token.kind == TOK_IDENTIFIER) {
    if (ctx->next_token.kind == TOK_COLON) {
      return parse_declaration(ctx);
    }
  }

  struct AstNode *expr = parse_expression(ctx, 0);

  // it's statement
  if (ctx->current_token.kind == TOK_SEMICOLON) {
    parser_advance(ctx);

    struct AstNode *stmt = astnode_new(ctx, AST_EXPRESSION_STATEMENT);
    stmt->as.expression_statement.expression = expr;

    return stmt;
  }

  // it's a trailing_expr
  if (ctx->current_token.kind == TOK_RBRACE) {
    block->as.block.trailing_expression = expr;
    return NULL;
  }

  if (ctx->current_token.kind == TOK_EQUAL) {
    if (!parser_is_valid_lvalue(expr)) {
      struct Error err = {
          .kind = ERR_INVALID_LVALUE,
          .span = expr->span,
      };

      print_error(err, ctx->lexer->file_name, ctx->lexer->contents);
      ctx->error_count++;
      parser_synchronise(ctx);

      return NULL;
    }

    parser_expect(ctx, TOK_EQUAL);
    struct AstNode *rhs = parse_expression(ctx, 0);

    struct Span end = ctx->current_token.span;
    parser_expect(ctx, TOK_SEMICOLON);

    struct AstNode *stmt = astnode_new(ctx, AST_ASSIGNMENT);

    stmt->as.assignment.lvalue = expr;
    stmt->as.assignment.rvalue = rhs;
    stmt->span = merge_span(expr->span, end);

    return stmt;
  }

  struct Error err = {
      .kind = ERR_SYNTAX,
      .span = ctx->current_token.span,
      .as.syntax.found = token_kind_str(ctx->current_token.kind),
  };
  print_error(err, ctx->lexer->file_name, ctx->lexer->contents);
  ctx->error_count++;

  parser_synchronise(ctx);
  return NULL;
}

