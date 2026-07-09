#ifndef TYPES_H
#define TYPES_H

enum TypeKind {
  TYPE_I8,
  TYPE_I32,
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
extern struct Type type_unknown;

#endif
