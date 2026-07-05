
#include "ast.h"
#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static void advance(Parser *p) {
  p->current_token = p->next_token;
  p->next_token = next_token(p->lexer);
}

static void synchronise(Parser *p, TokenKind kind) {
  // error recovery
  while (p->current_token.kind != TOK_EOF) {
    if (p->current_token.kind == kind ||
        p->current_token.kind == TOK_SEMICOLON) {
      advance(p);
      break;
    }
    advance(p);
  }
}
static void expect(Parser *p, TokenKind kind) {
  if (p->current_token.kind == kind) {
    advance(p);
    return;
  }

  fprintf(stderr, "Syntax error at %d:%d. Expected %s but got %s\n",
          p->lexer->line, p->lexer->col, kind_str(kind),
          kind_str(p->current_token.kind));
  p->has_error = true;

  synchronise(p, kind);

  return;
}

static AstNode *parse_expr(Parser *p) {
  switch (p->current_token.kind) {
  case TOK_INT_LITERAL: {
    Token int_tok = p->current_token;
    expect(p, TOK_INT_LITERAL);

    AstNode *node = malloc(sizeof(AstNode));

    node->kind = AST_INT_LITERAL;
    node->as.int_literal.value = atoll(int_tok.val);

    return node;
  }
  default:
    fprintf(stderr,
            "Syntax error at %d:%d: Expected an expression, but found %s\n",
            p->lexer->line, p->lexer->col, kind_str(p->current_token.kind));
    return NULL;
  }
}

static AstNode *parse_return_stmt(Parser *p) {
  expect(p, TOK_KW_RETURN);

  AstNode *return_stmt = malloc(sizeof(AstNode));
  return_stmt->kind = AST_RETURN_STMT;

  if (p->current_token.kind != TOK_SEMICOLON) {
    return_stmt->as.return_stmt.expr = parse_expr(p);
  }

  expect(p, TOK_SEMICOLON);

  return return_stmt;
}

static AstNode *parse_block(Parser *p, char *name) {
  expect(p, TOK_LBRACE);

  int capacity = 10;
  AstNode **statements = malloc(sizeof(AstNode *) * capacity);
  int count = 0;

  while (p->current_token.kind != TOK_EOF &&
         p->current_token.kind != TOK_RBRACE) {
    if (count >= capacity) {
      capacity *= 2;
      statements = realloc(statements, sizeof(AstNode *) * capacity);
    }
    AstNode *stmt = NULL;
    switch (p->current_token.kind) {
    case TOK_KW_RETURN:
      stmt = parse_return_stmt(p);
      break;
    default:
      fprintf(stderr, "Syntax error at %d:%d: Unexpected %s in block.\n",
              p->lexer->line, p->lexer->col, kind_str(p->current_token.kind));
      synchronise(p, TOK_SEMICOLON);
      continue;
    }
    if (stmt != NULL) {
      statements[count++] = stmt;
    }
  }

  AstNode *block = malloc(sizeof(AstNode));

  block->kind = AST_BLOCK_DECL;
  block->as.block.name = name;
  block->as.block.statements = statements;
  block->as.block.count = count;

  expect(p, TOK_RBRACE);
  return block;
}

static AstNode *parse_type(Parser *p) {
  AstNode *type = malloc(sizeof(AstNode));
  type->kind = AST_TYPE_UNKNOWN;
  if (p->current_token.kind == TOK_IDENTIFIER) {
    Token name_tok = p->current_token;
    expect(p, TOK_IDENTIFIER);

    type->kind = AST_TYPE_NAMED;
    type->as.type_named.name = name_tok.val;
  }
  if (type->kind == AST_TYPE_UNKNOWN) {
    fprintf(stderr, "Unexpected type casting at %d: %d\n", p->lexer->line,
            p->lexer->col);
    p->has_error = true;
  }
  return type;
}

static AstNode *parse_func_decl(Parser *p, char *name) {
  expect(p, TOK_KW_FUNC);

  expect(p, TOK_LPARAN);
  expect(p, TOK_RPARAN);

  AstNode *return_type = parse_type(p);
  AstNode *block = parse_block(p, name);

  AstNode *func = malloc(sizeof(AstNode));
  func->kind = AST_FUNCTION_DECL;

  func->as.function.name = name;
  func->as.function.return_type = return_type;
  func->as.function.block = block;

  return func;
}

static AstNode *parse_declaration(Parser *p) {
  Token name_tok = p->current_token;
  expect(p, TOK_IDENTIFIER);
  expect(p, TOK_COLON);

  if (p->current_token.kind == TOK_KW_FUNC) {
    return parse_func_decl(p, name_tok.val);
  }
  return NULL;
}

AstNode *parse_program(Parser *p) {
  int capacity = 10;
  int count = 0;

  AstNode **declaration = malloc(sizeof(AstNode *) * capacity);

  while (p->current_token.kind != TOK_EOF) {
    if (count >= capacity) {
      capacity *= 2;
      declaration = realloc(declaration, sizeof(AstNode *) * capacity);
    }
    if (p->current_token.kind == TOK_IDENTIFIER) {
      declaration[count++] = parse_declaration(p);
    } else {
      fprintf(stderr, "Unexpected token %s at top level, %d:%d\n",
              kind_str(p->current_token.kind), p->lexer->line, p->lexer->col);
      advance(p);
    }
  }

  AstNode *program = malloc(sizeof(AstNode));

  program->kind = AST_PROGRAM;
  program->as.program.count = count;
  program->as.program.declaration = declaration;

  return program;
}

Parser parser_init(Lexer *l) {
  Parser p = (Parser){
      .lexer = l,
      .has_error = false,
  };

  p.current_token = next_token(l);
  p.next_token = next_token(l);

  return p;
}

static void indentation(int indent) {
  for (int i = 0; i < indent; i++)
    printf("  ");
}

void print_ast(AstNode *node, int indent) {
  if (node == NULL)
    return;

  indentation(indent);

  switch (node->kind) {
  case AST_PROGRAM: {
    printf("AstProgram: \n");
    for (int i = 0; i < node->as.program.count; i++) {
      print_ast(node->as.program.declaration[i], indent + 1);
    }
    break;
  }
  case AST_FUNCTION_DECL:
    printf("AstFunctionDecl: (name: %s)\n", node->as.function.name);
    print_ast(node->as.function.return_type, indent + 1);
    print_ast(node->as.function.block, indent + 1);
    break;
  case AST_TYPE_UNKNOWN:
    printf("AstUnknownType: \n");
    break;
  case AST_TYPE_NAMED:
    printf("AstTypeNamed: (name: %s)\n", node->as.type_named.name);
    break;
  case AST_BLOCK_DECL:
    printf("AstBlockDecl: (name: %s)\n", node->as.block.name);
    for (int i = 0; i < node->as.block.count; i++) {
      print_ast(node->as.block.statements[i], indent + 1);
    }
    break;
  case AST_RETURN_STMT:
    printf("AstReturnStmt:\n");
    print_ast(node->as.return_stmt.expr, indent + 1);
    break;
  case AST_INT_LITERAL:
    printf("AstIntLiteral: (value: %lld)\n", node->as.int_literal.value);
    break;
  }
}
