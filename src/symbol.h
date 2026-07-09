#ifndef SYMBOL_H
#define SYMBOL_H

enum SymbolKind {

};

struct Symbol {
  SymbolKind kind;
  union{} as;
};
#endif
