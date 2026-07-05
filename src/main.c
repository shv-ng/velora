#include "ast.h"
#include "file.h"
#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "zyn: error: no input file\nusage:\n  %s [path]\n",
            argv[0]);
    return 1;
  }

  char *file_name = argv[1];

  intmax_t file_size = get_file_size(file_name);

  char *contents = read_file(file_name, file_size);
  if (contents == NULL)
    return 1;

  printf("%s\n\n", contents);

  struct Lexer l = lexer_init(file_name, contents);

  // struct Token t;
  // while ((t = next_token(&l)).kind != TOK_EOF) {
  //   printf("line %d | kind %s | val %s | \n", t.line, kind_str(t.kind),
  //   t.val); if (t.val) {
  //     free(t.val);
  //   }
  // }

  struct Parser p = parser_init(&l);

  struct AstNode *program = parse_program(&p);
  if (p.error_count == 0) {
    print_ast(program, 0);
  } else {
    fprintf(stderr, "%d error generated\n", p.error_count);
  }

  free(contents);

  return 0;
}
