#ifndef AST_H
#define AST_H

#include "lexer.h"
#include <stdbool.h>
#include <stdint.h>

enum AstKind {
  AST_PROGRAM,
  AST_FUNCTION_DECL, // fn(...)...{}

  AST_TYPE_UNKNOWN,
  AST_TYPE_NAMED, // i32, User

  AST_BLOCK_DECL, // {...}

  AST_RETURN_STMT, // return ...;
  AST_INT_LITERAL, // 42
};

struct AstIntLiteral {
  long long value;
};

struct AstReturnStmt {
  struct AstNode *expr;
};

struct AstBlockDecl {
  struct AstNode **statements;
  char *name;
  int count;
};

struct AstTypeNamed {
  char *name;
};

struct AstFunctionDecl {
  char *name;
  struct AstNode *return_type;
  struct AstNode *block;
};

struct AstProgram {
  int count;
  struct AstNode **declaration;
};

struct AstNode {
  enum AstKind kind;

  union {
    struct AstProgram program;
    struct AstFunctionDecl function;
    struct AstTypeNamed type_named;
    struct AstBlockDecl block;
    struct AstReturnStmt return_stmt;
    struct AstIntLiteral int_literal;
  } as;
};

struct Parser {
  struct Lexer *lexer;
  struct Token current_token;
  struct Token next_token;
  int error_count;
};

struct Parser parser_init(struct Lexer *l);

struct AstNode *parse_program(struct Parser *p);

void print_ast(struct AstNode *node, int indent);

#endif
