
#include <stdio.h>

void print_help(char *argv[]) {
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
