#include "parser_internal.h"

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
  int capacity = 10;
  int count = 0;

  struct AstNode **declaration =
      arena_malloc(p->arena, sizeof(struct AstNode *) * capacity);

  while (p->current_token.kind != TOK_EOF) {
    if (count >= capacity) {
      size_t old_size = capacity * sizeof(struct AstNode *);

      capacity *= 2;
      size_t new_size = capacity * sizeof(struct AstNode *);
      declaration = arena_realloc(p->arena, declaration, old_size, new_size);
    }
    if (p->current_token.kind == TOK_IDENTIFIER) {
      struct AstNode *decl = parse_declaration(p);
      if (decl != NULL) {
        declaration[count++] = decl;
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
