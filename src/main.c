#include "ast.h"
#include "file.h"
#include "lexer.h"
#include "sema.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "zyn: error: no input file\nusage:\n  %s [path]\n",
            argv[0]);
    return 1;
  }

  char *file_name = argv[1];

  intmax_t file_size = file_get_size(file_name);

  char *contents = file_read(file_name, file_size);
  if (contents == NULL)
    return 1;

  printf("%s\n\n", contents);

  struct Lexer l = lexer_new(file_name, contents);

  // struct Token t;
  // while ((t = next_token(&l)).kind != TOK_EOF) {
  //   printf("line %d | kind %s | val %s | \n", t.line, kind_str(t.kind),
  //   t.val); if (t.val) {
  //     free(t.val);
  //   }
  // }

  struct Parser p = parser_new(&l);

  struct AstNode *program = parse_program(&p);
  if (p.error_count != 0) {
    fprintf(stderr, "%d error generated\n", p.error_count);
    exit(1);
  }

  struct SemaCtx sema = sema_new(&p);
  sema_check(&sema, program);

  print_ast(program, 0);

  free(contents);

  return 0;
}
