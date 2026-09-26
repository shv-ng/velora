#pragma once

// operator which need only one operand, ofc
// like -
enum UnaryOperator {
  OP_UNKNOWN_UNARY_OP,
  OP_NEGATIVE,
  OP_BITWISE_NOT,
};

// ast node for unary
struct AstUnaryExpression {
  struct AstNode *expression;
  enum UnaryOperator op;
  bool is_prefix; // it's for saying, is it is_prefix like -x or not maybe add later  
};
