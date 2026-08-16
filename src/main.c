#include "ast.h"
#include "cli.h"
#include "file.h"
#include "lexer.h"
#include "sema.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  return cli_new(argc, argv);

  // char *file_name = argv[1];
  //
  // intmax_t file_size = file_get_size(file_name);
  //
  // char *contents = file_read(file_name, file_size);
  // if (contents == NULL)
  //   return 1;
  //
  // printf("%s\n\n", contents);
  //
  // struct Lexer l = lexer_new(file_name, contents);
  //
  // struct Parser p = parser_new(&l);
  //
  // struct AstNode *program = parse_program(&p);
  // if (p.error_count != 0) {
  //   fprintf(stderr, "%d error generated\n", p.error_count);
  //   exit(1);
  // }
  //
  // struct SemaCtx sema = sema_new(&p);
  // sema_check(&sema, program);
  //
  // print_ast(program, 0);
  //
  // free(contents);
  //
  return 0;
}
