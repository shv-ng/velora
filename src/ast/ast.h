#pragma once

#include "../token/token.h"
#include "../types/types.h"
#include "ast_assignment.h"
#include "ast_binary_expression.h"
#include "ast_block_declaration.h"
#include "ast_expression_statement.h"
#include "ast_function_declaration.h"
#include "ast_identifier.h"
#include "ast_literals.h"
#include "ast_program.h"
#include "ast_return_statement.h"
#include "ast_types.h"
#include "ast_unary_expression.h"
#include "ast_variable_declaration.h"

struct AstNode;
struct Symbol;

// kind of ast nodes,
enum AstKind {
  AST_PROGRAM,
  AST_FUNCTION_DECLARATION, // fn(...)...{}
  AST_VARIABLE_DECLARATION, // x:i32 = 69;
  AST_ASSIGNMENT,           // x = 42;

  AST_TYPE_UNKNOWN,
  AST_TYPE_NAMED, // i32, User

  AST_BLOCK_DECLARAION, // {...}

  AST_RETURN_STATEMENT,     // return ...;
  AST_EXPRESSION_STATEMENT, // expr ;

  AST_INT_LITERAL, // 42
  AST_IDENTIFIER,  // x

  AST_BINARY_EXPRESSION, // .. op ..
  AST_UNARY_EXPRESSION,  //  op ..   | .. op
};

// unified struct for all ast nodes
struct AstNode {
  enum AstKind kind;
  struct Type *resolved_type;
  struct Span span;
  struct Symbol *symbol;

  union {
    struct AstProgram program;
    struct AstFunctionDeclaration function;
    struct AstVariableDeclaration variable_declaration; 
    struct AstTypeNamed type_named;
    struct AstBlockDeclaration block;
    struct AstReturnStatement return_statement;
    struct AstExpressionStatement expression_statement;
    struct AstIntLiteral int_literal;
    struct AstBinaryExpression binary_expression;
    struct AstUnaryExpression unary_expression;
    struct AstIdentifier identifer;
    struct AstAssignment assignment;
  } as;
};

// It'll take node, and print it into recursive tree format
// helpful in debuging
void ast_print_tree(struct AstNode *node, int indent);
