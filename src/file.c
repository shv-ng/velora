#include "file.h"

#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

intmax_t file_get_size(char *file_name) {
  struct stat sb;

  if (lstat(file_name, &sb) == -1) {
    perror("lstat");
    exit(EXIT_FAILURE);
  }
  return (intmax_t)sb.st_size;
}

char *file_read(struct Arena *a, char *file_name, intmax_t file_size) {

  char *contents = arena_malloc(a, file_size + 1);
  if (!contents) {
    return NULL;
  }

  contents[file_size] = '\0';

  FILE *file = fopen(file_name, "rb");
  if (!file) {
    perror("fopen");
    return NULL;
  }

  long bytes_read = fread(contents, sizeof(char), file_size, file);
  if (bytes_read != file_size) {
    fprintf(stderr,
            "Short read of '%s': expected %jd bytes "
            "but got %zu: %s.\n",
            file_name, file_size, bytes_read, strerror(errno));

    contents = NULL;
  }

  fclose(file);

  return contents;
}
