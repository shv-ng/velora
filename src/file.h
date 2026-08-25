#pragma once

#include "arena.h"
#include <stdint.h>

char *file_read(struct Arena *a, char *file_name, intmax_t file_size);
intmax_t file_get_size(char *file_name);
