#include "ast.h"
#include "file.h"
#include "lexer.h"
#include "sema.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#ifndef VERSION
#define VERSION "dev"
#endif

#ifndef COMMIT
#define COMMIT "unknown"
#endif

#ifndef DATE
#define DATE "unknown"
#endif

#include <stdio.h>

static int execute_run(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "<path> is missing\n");
    return 1;
  }

  char *file_name = argv[1];
  intmax_t file_size = file_get_size(file_name);

  char *contents = file_read(file_name, file_size);
  if (contents == NULL)
    return 1;

  printf("%s\n\n", contents);

  struct Lexer l = lexer_new(file_name, contents);

  struct Parser p = parser_new(&l);

  struct AstNode *program = parse_program(&p);
  if (p.error_count != 0) {
    fprintf(stderr, "%d error generated\n", p.error_count);
    return 1;
  }

  struct SemaCtx sema = sema_new(&p);
  sema_check(&sema, program);

  print_ast(program, 0);

  free(contents);

  return 0;
}

static void print_help(char *argv[]) {
  fprintf(stderr, "usage:\n");
  fprintf(stderr, "    %s <command> [arguments]\n", argv[0]);

  fprintf(stderr, "\n");

  fprintf(stderr, "commands:\n");
  fprintf(stderr, "    run <path>       compile and run program\n");
  fprintf(stderr, "    help             print this msg\n");
  fprintf(stderr, "    version          print version\n");

  fprintf(stderr, "\n");

  fprintf(stderr, "arguments:\n");
  fprintf(stderr, "    -h, --help       print this msg\n");
  fprintf(stderr, "    -v, --version    print version\n");
}

static void print_version() {
  fprintf(stdout, "velora %s (%s %s)", VERSION, COMMIT, DATE);
}

int cli_new(int argc, char *argv[]) {

  if (argc < 2 || strcmp(argv[1], "help") == 0 || strcmp(argv[1], "-h") == 0 ||
      strcmp(argv[1], "--help") == 0) {
    print_help(argv);
    return 0;
  }

  if (strcmp(argv[1], "version") == 0 || strcmp(argv[1], "-v") == 0 ||
      strcmp(argv[1], "--version") == 0) {
    print_version();
    return 0;
  }

  if (strcmp(argv[1], "run") == 0) {
    return execute_run(argc - 1, argv + 1);
  }

  return 0;
}
