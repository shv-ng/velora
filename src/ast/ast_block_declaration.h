#pragma once

struct AstBlockDeclaration {
  struct AstNode **statements;
  struct AstNode *trailing_expression;
  char *name;
  int count;
};
