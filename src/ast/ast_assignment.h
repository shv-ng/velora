#pragma once

// ast node that is use for x = 3;
// use for reassign value, not for declaration
// `lvalue` must be any value assignable value
struct AstAssignment {
  struct AstNode *lvalue;
  struct AstNode *rvalue;
};
