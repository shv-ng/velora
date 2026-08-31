#pragma once

enum UnaryOp {
  OP_UNKNOWN_UNARY_OP,
  OP_NEGATIVE,
  OP_BITWISE_NOT,
};

struct AstUnaryExpr {
  struct AstNode *expr;
  enum UnaryOp op;
  bool is_prefix;
};
