#pragma once

// a program (maybe pkg? decided later, for now, keep program)
struct AstProgram {
  int count;
  struct AstNode **declaration;
};
