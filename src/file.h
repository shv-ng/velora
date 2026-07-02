#ifndef FILE_H
#define FILE_H

#include <stdint.h>

intmax_t get_file_size(char *file_name);
char *read_file(char *file_name, intmax_t file_size);

#endif
