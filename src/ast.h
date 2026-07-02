#ifndef AST_H
#define AST_H

#include "lexer.h"
#include <stdbool.h>

typedef struct AstNode AstNode;

typedef enum {
  AST_PROGRAM,
  AST_FUNCTION_DECL,

  AST_TYPE_UNKNOWN,
  AST_TYPE_NAMED,
} AstKind;

typedef struct {
  char *name;
} AstTypeNamed;

typedef struct {
  char *name;
  AstNode *return_type;
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
