#pragma once

// ast for block, contains all the statements/declarations
// it can have a name, that will use for scope name/arena name etc
// the trailing_expr decided it's type
struct AstBlockDeclaration {
  struct AstNode **statements;
  struct AstNode *trailing_expression;
  char *name;
  int count;
};
