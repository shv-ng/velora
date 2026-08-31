#pragma once

#include "../token/token.h"
#include "../types/types.h"
#include "ast_binary_expr.h"
#include "ast_block.h"
#include "ast_expr_stmt.h"
#include "ast_function.h"
#include "ast_literals.h"
#include "ast_program.h"
#include "ast_return_stmt.h"
#include "ast_types.h"
#include "ast_unary_expr.h"

struct AstNode;

enum AstKind {
  AST_PROGRAM,
  AST_FUNCTION_DECL, // fn(...)...{}

  AST_TYPE_UNKNOWN,
  AST_TYPE_NAMED, // i32, User

  AST_BLOCK_DECL, // {...}

  AST_RETURN_STMT, // return ...;
  AST_EXPR_STMT,   // expr ;

  AST_INT_LITERAL, // 42

  AST_BINARY_EXPR, // .. op ..
  AST_UNARY_EXPR,  //  op ..   | .. op
};

struct AstNode {
  enum AstKind kind;
  struct Type *resolved_type;
  struct Span span;

  union {
    struct AstProgram program;
    struct AstFunctionDecl function;
    struct AstTypeNamed type_named;
    struct AstBlockDecl block;
    struct AstReturnStmt return_stmt;
    struct AstExprStmt expr_stmt;
    struct AstIntLiteral int_literal;
    struct AstBinaryExpr binary_expr;
    struct AstUnaryExpr unary_expr;
  } as;
};

void print_ast(struct AstNode *node, int indent);
