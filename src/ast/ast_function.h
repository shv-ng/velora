#pragma once

struct AstFunctionDecl {
  char *name;
  struct AstNode *return_type;
  struct AstNode *block;
};
