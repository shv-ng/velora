#include "ast.h"
#include "codegen.h"
#include "file.h"
#include "lexer.h"
#include "sema.h"
#include "symbol.h"
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

  int err_code = 0;
  int err_count = 0;

  struct Lexer lexer = lexer_new(file_name, contents);

  struct Parser parser = parser_new(&lexer);

  struct AstNode *program_ast = parse_program(&parser);
  if (parser.error_count != 0) {
    err_count += parser.error_count;
    err_code = 1;
    goto content_cleanup;
  }

  struct SemaCtx sema = sema_new(&parser);
  sema_check(&sema, program_ast);
  if (sema.error_count != 0) {
    err_count += sema.error_count;
    err_code = 1;
    goto scope_cleanup;
  }

  struct CodegenCtx codegen = codegen_new(&sema);
  codegen_emit(&codegen, program_ast);

  if (codegen.error_count != 0) {
    err_count += codegen.error_count;
    err_code = 1;
    goto ast_cleanup;
  }

  codegen_free(&codegen);

scope_cleanup:
  scope_free(sema.current_scope);

ast_cleanup:
  ast_free(program_ast);

content_cleanup:
  free(contents);

  if (err_code == 1) {
    fprintf(stderr, "%d error generated\n", err_count);
  }
  return err_code;
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
