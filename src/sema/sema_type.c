#include "sema_internal.h"
#include <string.h>

struct Type *resolve_type_node(struct AstNode *node) {
  struct Type *t = &type_unknown;
  switch (node->kind) {
  case AST_TYPE_NAMED: {
    if (strcmp(node->as.type_named.name, "i32") == 0) {
      t = &type_i32;
    } else if (strcmp(node->as.type_named.name, "i8") == 0) {
      t = &type_i8;
    }
    break;
  }
  default:
    break;
  }
  return t;
}
