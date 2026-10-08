// when it'll intruduced, it'll do few things
// - divergence : a block get break/continue/return/inf loop/panic
// - const folding in frontend
// - ownership etc
#pragma once

enum MirOp {
  MIR_CONST,
  MIR_RETURN,
};

struct MirInstr {
  enum MirOp op;
  int dst;
  int src;
  long val;
};

struct MirBlock {
  int id;
  struct MirInstr *instrs;
  int count;
};

struct MirFunc {
  char *name;
  struct MirBlock *blocks;
  int count;
};
