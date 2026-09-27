#include "file.h"
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

char *file_read(struct Arena *a, char *file_name) {
  struct stat sb;

  if (lstat(file_name, &sb) == -1) {
    emit_error(NULL, NO_SPAN, ERR_FILE, strerror(errno));
    return NULL;
  }

  intmax_t file_size = (intmax_t)sb.st_size;

  char *contents = arena_malloc(a, file_size + 1);
  contents[file_size] = '\0';

  FILE *file = fopen(file_name, "rb");
  if (!file) {
    emit_error(NULL, NO_SPAN, ERR_FILE, strerror(errno));
    return NULL;
  }

  long bytes_read = fread(contents, sizeof(char), file_size, file);
  if (bytes_read != file_size) {
    emit_error(NULL, NO_SPAN, ERR_FILE, strerror(errno));
    return NULL;
  }

  fclose(file);

  return contents;
}
