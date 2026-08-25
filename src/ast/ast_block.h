#pragma once

struct AstBlockDecl {
  struct AstNode **statements;
  char *name;
  int count;
};
