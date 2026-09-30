#pragma once

struct AstIntLiteral {
  const char *raw;
  unsigned long long value;
};

struct AstBoolLiteral {
  bool is_true;
};
