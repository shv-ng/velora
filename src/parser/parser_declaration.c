#include "parser_internal.h"

struct AstNode *parse_declaration(struct ParserCtx *ctx) {
  struct Token name_tok = ctx->current_token;

  parser_expect(ctx, TOK_IDENTIFIER);
  parser_expect(ctx, TOK_COLON);

  switch (ctx->current_token.kind) {
  case TOK_KW_FUNC:
    return parse_function_declaration(ctx, name_tok.val);
  case TOK_IDENTIFIER: {
    struct AstNode *type = parse_type(ctx);
    if (ctx->current_token.kind == TOK_EQUAL) {
      return parse_variable_declaration(ctx, name_tok.val, type, name_tok.span);
    }
  }

  default: {
    struct Error err = {
        .kind = ERR_SYNTAX,
        .span = ctx->current_token.span,
        .as.syntax.found = token_kind_str(ctx->current_token.kind),
    };

    print_error(err, ctx->lexer->file_name, ctx->lexer->contents);
    ctx->error_count++;

    parser_advance(ctx);
    return NULL;
  }
  }
}
