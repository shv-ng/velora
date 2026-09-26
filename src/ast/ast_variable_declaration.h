#pragma once

// a varible declared 
// x: i32 = 42;
struct AstVariableDeclaration {
  char *name;
  struct AstNode *type; 
  struct AstNode *expression;
};
