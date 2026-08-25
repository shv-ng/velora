#pragma once
#include "arena.h"

void da_append(struct Arena *a, void ***array, void *value, size_t *count,
               size_t *capacity);
