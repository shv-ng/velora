#pragma once

struct AstFunctionDeclaration {
  char *name; // function name
  struct AstNode *return_type;
  struct AstNode *block;
};
