#ifndef MIR_H
#define MIR_H

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

#endif
