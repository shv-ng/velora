#ifndef FILE_H
#define FILE_H

#include <stdint.h>

intmax_t file_get_size(char *file_name);
char *file_read(char *file_name, intmax_t file_size);

#endif
