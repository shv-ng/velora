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

static void print_help(char *argv[]) {
  fprintf(stderr, "usage:\n");
  fprintf(stderr, "    %s <command> [arguments]\n", argv[0]);

  fprintf(stderr, "\n");

  fprintf(stderr, "commands:\n");
  fprintf(stderr, "    run              compile and run program\n");
  fprintf(stderr, "    help             print this msg\n");
  fprintf(stderr, "    version          print version\n");

  fprintf(stderr, "\n");

  fprintf(stderr, "arguments:\n");
  fprintf(stderr, "    -h, --help       print this msg\n");
  fprintf(stderr, "    -v, --version    print version\n");
}

static void print_version(char *argv[]) {
  fprintf(stdout, "%s %s (%s %s)", argv[0], VERSION, COMMIT, DATE);
}

int cli_new(int argc, char *argv[]) {

  if (argc < 2 || strcmp(argv[1], "help") == 0 || strcmp(argv[1], "-h") == 0 ||
      strcmp(argv[1], "--help") == 0) {
    print_help(argv);
    return 0;
  }

  if (strcmp(argv[1], "version") == 0 || strcmp(argv[1], "-v") == 0 ||
      strcmp(argv[1], "--version") == 0) {
    print_version(argv);
    return 0;
  }

  return 0;
}
