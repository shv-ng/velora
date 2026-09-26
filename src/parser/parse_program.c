#include "../utils/da.h"
#include "parser_internal.h"

struct AstNode *parse_program(struct ParserCtx *ctx) {
  size_t capacity = 10;
  size_t count = 0;

  struct AstNode **declaration =
      arena_malloc(ctx->arena, sizeof(struct AstNode *) * capacity);

  while (ctx->current_token.kind != TOK_EOF) {
    if (ctx->current_token.kind == TOK_IDENTIFIER) {

      struct AstNode *decl = parse_declaration(ctx);

      if (decl != NULL) {
        da_append(ctx->arena, (void ***)&declaration, (void *)decl,
                  &count, &capacity);
      }

    } else {
      struct Error err = {
          .kind = ERR_SYNTAX,
          .span = ctx->current_token.span,
          .as.syntax.found = token_kind_str(ctx->current_token.kind),
      };
      print_error(err, ctx->lexer->file_name, ctx->lexer->contents);
      ctx->error_count++;

      parser_advance(ctx);
    }
  }

  struct AstNode *program = astnode_new(ctx, AST_PROGRAM);

  program->as.program.count = count;
  program->as.program.declaration = declaration;

  return program;
}
