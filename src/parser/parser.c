#include "parser_internal.h"
#include <stddef.h>

struct Parser parser_new(struct Lexer *l) {
  struct Parser p = (struct Parser){
      .lexer = l,
      .error_count = 0,
      .arena = l->arena,
  };

  p.current_token = next_token(l);
  p.next_token = next_token(l);

  return p;
}

struct AstNode *parse_program(struct Parser *p) {
  size_t capacity = 10;
  size_t count = 0;

  struct AstNode **declaration =
      arena_malloc(p->arena, sizeof(struct AstNode *) * capacity);

  while (p->current_token.kind != TOK_EOF) {
    if (p->current_token.kind == TOK_IDENTIFIER) {
      struct AstNode *decl = parse_declaration(p);

      if (decl != NULL) {
        da_append(p->arena, (void ***)&declaration, (void *)decl, &count,
                  &capacity);
      }

    } else {
      struct Error err = {
          .kind = ERR_SYNTAX,
          .span = p->current_token.span,
          .as.syntax.found = token_kind_str(p->current_token.kind),
      };
      print_error(err, p->lexer->file_name, p->lexer->contents);
      p->error_count++;

      advance(p);
    }
  }

  struct AstNode *program = astnode_new(p, AST_PROGRAM);
  program->as.program.count = count;
  program->as.program.declaration = declaration;

  return program;
}
