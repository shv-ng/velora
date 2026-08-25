#pragma once

#include <stdbool.h>

enum TypeKind {
  TYPE_I8,
  TYPE_I32,
  TYPE_VOID,
  TYPE_UNKNOWN,
  TYPE_FUNC,
};

struct TypeFunc {
  struct Type **params;
  int param_count;
  struct Type *return_type;
};

struct Type {
  enum TypeKind kind;
  union {
    struct TypeFunc func;
  } as;
};

extern struct Type type_i8;
extern struct Type type_i32;
extern struct Type type_void;
extern struct Type type_unknown;

bool type_equal(struct Type *a, struct Type *b);
const char *type_str(struct Type *a);
