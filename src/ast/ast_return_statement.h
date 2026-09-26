#pragma once

// return statement
// return ...;
struct AstReturnStatement {
  struct AstNode *expression;
};
