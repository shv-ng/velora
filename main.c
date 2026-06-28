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

  Lexer l = lexer_init(file_name, contents);
  Token t;
  while ((t = next_token(&l)).kind != TOK_EOF) {
    printf("line %d | kind %s | \n", t.line, kind_str(t.kind));
  }

  free(contents);

  return 0;
}
