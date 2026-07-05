#ifndef AST_H
#define AST_H

#include "lexer.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct AstNode AstNode;

typedef enum {
  AST_PROGRAM,
  AST_FUNCTION_DECL, // fn(...)...{}

  AST_TYPE_UNKNOWN,
  AST_TYPE_NAMED, // i32, User

  AST_BLOCK_DECL, // {...}

  AST_RETURN_STMT, // return ...;
  AST_INT_LITERAL, // 42
} AstKind;

typedef struct {
  long long value;
} AstIntLiteral;

typedef struct {
  AstNode *expr;
} AstReturnStmt;

typedef struct {
  AstNode **statements;
  char *name;
  int count;
} AstBlockDecl;

typedef struct {
  char *name;
} AstTypeNamed;

typedef struct {
  char *name;
  AstNode *return_type;
  AstNode *block;
} AstFunctionDecl;

typedef struct {
  int count;
  AstNode **declaration;
} AstProgram;

typedef struct AstNode {
  AstKind kind;

  union {
    AstProgram program;
    AstFunctionDecl function;
    AstTypeNamed type_named;
    AstBlockDecl block;
    AstReturnStmt return_stmt;
    AstIntLiteral int_literal;
  } as;
} AstNode;

typedef struct {
  Lexer *lexer;
  Token current_token;
  Token next_token;
  bool has_error;
} Parser;

Parser parser_init(Lexer *l);

AstNode *parse_program(Parser *p);

void print_ast(AstNode *node, int indent);

#endif
