#pragma once

enum BinaryOp {
  OP_UNKNOWN_BINARY_OP,
  OP_ADD,
  OP_SUB,
  OP_MULIPLY,
  OP_DIVIDE,
  OP_MODULO,
  OP_LEFT_SHIFT,
  OP_RIGHT_SHIFT,
  OP_BITWISE_XOR,
  OP_BITWISE_OR,
  OP_BITWISE_AND,
};

struct AstBinaryExpr {
  struct AstNode *left;
  struct AstNode *right;
  enum BinaryOp op;
};
