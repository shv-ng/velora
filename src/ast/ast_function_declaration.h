#pragma once

// a func declaration, name say
// main: fn() 
struct AstFunctionDeclaration {
  char *name; // function name
  struct AstNode *return_type;
  struct AstNode *block;
};
