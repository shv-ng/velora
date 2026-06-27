#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>

intmax_t get_file_size(char *file_name) {
  struct stat sb;

  if (lstat(file_name, &sb) == -1) {
    perror("lstat");
    exit(EXIT_FAILURE);
  }
  return (intmax_t)sb.st_size;
}

char *read_file(char *file_name, intmax_t file_size) {
  int fail = 0;

  char *contents = malloc(file_size + 1);
  if (contents == NULL) {
    perror("malloc");
    return NULL;
  }

  contents[file_size] = '\0';

  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    perror("fopen");
    free(contents);
    return NULL;
  }

  size_t bytes_read = fread(contents, sizeof(char), file_size, file);
  if (bytes_read != file_size) {
    fprintf(stderr,
            "Short read of '%s': expected %jd bytes "
            "but got %zu: %s.\n",
            file_name, file_size, bytes_read, strerror(errno));

    free(contents);
    contents = NULL;

    goto free_file;
  }

free_file:
  fclose(file);

  return contents;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "zyn: error: no input file\nusage:\n  %s [path]\n",
            argv[0]);
    return 1;
  }

  char *file_name = argv[1];

  intmax_t file_size = get_file_size(file_name);
  printf("file size: %jd\n", file_size);

  char *contents = read_file(file_name, file_size);
  if (contents == NULL)
    return 1;

  printf("%s", contents);

  free(contents);

  return 0;
}
