#include "cli.h"
#include <stdint.h>
#include <stdio.h>
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

static int execute_build(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "<path> is missing\n");
    return 1;
  }

  char *file_name = argv[1];
  intmax_t file_size = file_get_size(file_name);

  struct Arena *a = arena_new();

  int err_code = 0;
  int err_count = 0;

  char *contents = file_read(a, file_name, file_size);

  if (!contents) {
    err_count += 1;
    err_code = 1;
    goto cleanup;
  }

  struct Lexer lexer = lexer_new(a, file_name, contents);
  struct Parser parser = parser_new(&lexer);

  struct AstNode *program_ast = parse_program(&parser);

  if (parser.error_count != 0) {
    err_count += parser.error_count;
    err_code = 1;
    goto cleanup;
  }

  struct SemaCtx sema = sema_new(&parser);
  sema_check(&sema, program_ast);

  if (sema.error_count != 0) {
    err_count += sema.error_count;
    err_code = 1;
    goto cleanup;
  }

  struct CodegenCtx codegen = codegen_new(&sema);
  codegen_emit(&codegen, program_ast);

  if (codegen.error_count != 0) {
    err_count += codegen.error_count;
    err_code = 1;
    goto cleanup;
  }

  codegen_free(&codegen);

cleanup:
  arena_free(a);

  if (err_code == 1) {
    fprintf(stderr, "%d error generated\n", err_count);
  }

  return err_code;
}

static int execute_run(int argc, char *argv[]) {
  int fail = execute_build(argc, argv);
  if (fail == 1) {
    return 1;
  }

  int status = system("/tmp/main");
  int actual_code = WEXITSTATUS(status);
  return actual_code;
}

static void print_help(char *argv[]) {
  fprintf(stderr, "usage:\n");
  fprintf(stderr, "    %s <command> [arguments]\n", argv[0]);

  fprintf(stderr, "\n");

  fprintf(stderr, "commands:\n");
  fprintf(stderr, "    run <path>       compile and run program\n");
  fprintf(stderr, "    build <path>     compile program\n");
  fprintf(stderr, "    help             print this msg\n");
  fprintf(stderr, "    version          print version\n");

  fprintf(stderr, "\n");

  fprintf(stderr, "arguments:\n");
  fprintf(stderr, "    -h, --help       print this msg\n");
  fprintf(stderr, "    -v, --version    print version\n");
}

static void print_version() {
  fprintf(stdout, "velora %s (%s %s)\n", VERSION, COMMIT, DATE);
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

  if (strcmp(argv[1], "build") == 0) {
    return execute_build(argc - 1, argv + 1);
  }

  if (strcmp(argv[1], "run") == 0) {
    return execute_run(argc - 1, argv + 1);
  }

  return 0;
}
