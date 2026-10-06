#pragma once

struct AstIfElseExpression {
  struct AstNode *condition;
  struct AstNode *if_block;
  struct AstNode *else_block;
};
