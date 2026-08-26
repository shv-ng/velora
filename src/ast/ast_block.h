#pragma once

struct AstBlockDecl {
  struct AstNode **statements;
  struct AstNode *trailing_expr;
  char *name;
  int count;
};
