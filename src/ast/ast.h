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

  AST_BLOCK_DECLARATION, // {...}

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

  // resolved after sema
  struct Type *resolved_type;

  struct Span span;
  
  // same, resolved after sema, use in codegen dropped the
  // use of lookup for symbol name in symbol table while codegen
  struct Symbol *symbol;

  union {
    // a program (maybe pkg? decided later, for now, keep program)
    struct AstProgram program;

    // a func declaration, name say main: fn()
    struct AstFunctionDeclaration function;

    // a varible declared x: i32 = 42;
    struct AstVariableDeclaration variable_declaration;

    // type, with name maybe i32, etc
    struct AstTypeNamed type_named;

    // ast for block, contains all the statements/declarations
    // it can have a name, that will use for scope name/arena name etc
    // the trailing_expr decided it's type
    struct AstBlockDeclaration block;

    // return statement
    // return ...;
    struct AstReturnStatement return_statement;

    // it's a expression ends with semicolon
    struct AstExpressionStatement expression_statement;

    // just number
    struct AstIntLiteral int_literal;

    // ast for expr contains recursive expr as rhs, and lhs and the op
    struct AstBinaryExpression binary_expression;

    // ast node for unary expression
    struct AstUnaryExpression unary_expression;

    // identifier used
    // x + ...
    struct AstIdentifier identifer;

    // ast node that is use for x = 3;
    // use for reassign value, not for declaration
    // `lvalue` must be any value assignable value
    struct AstAssignment assignment;
  } as;
};

// It'll take node, and print it into recursive tree format
// helpful in debuging
void ast_print_tree(struct AstNode *node, int indent);
