#pragma once

struct AstAssignment {
  struct AstNode *lvalue;
  struct AstNode *rvalue;
};
