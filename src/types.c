#include "types.h"
#include <stdbool.h>

struct Type type_i8 = {TYPE_I8, {}};
struct Type type_i32 = {TYPE_I32, {}};
struct Type type_unknown = {TYPE_UNKNOWN, {}};

bool type_equal(struct Type *a, struct Type *b) {
  if (a->kind != b->kind || a->kind == TYPE_UNKNOWN) {
    return false;
  }

  switch (a->kind) {
  case TYPE_FUNC:
    return type_equal(a->as.func.return_type, b->as.func.return_type);
  default:
    return true;
  }
}

const char *type_str(struct Type *t) {
  switch (t->kind) {
  case TYPE_I8:
    return "i8";
  case TYPE_I32:
    return "i32";
  case TYPE_UNKNOWN:
    return "unknown_type";
  case TYPE_FUNC:
    return "function";
  }
}
