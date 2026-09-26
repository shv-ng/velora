#pragma once

struct AstVariableDeclaration {
  char *name;
  struct AstNode *type; 
  struct AstNode *expression;
};
