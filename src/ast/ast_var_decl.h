#pragma once

struct AstVarDecl {
  char *name;
  struct AstNode *type;
  struct AstNode *expr;
};
