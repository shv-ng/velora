
#include "ast.h"
#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static void advance(Parser *p) {
  p->current_token = p->next_token;
  p->next_token = next_token(p->lexer);
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

  // // error recovery, skipping that line or block, whichever came first
  // while (p->current_token.kind != TOK_SEMICOLON &&
  //        p->current_token.kind != TOK_RBRACE)
  //   advance(p);
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

  AstNode *func = malloc(sizeof(AstNode));
  func->kind = AST_FUNCTION_DECL;

  func->as.function.name = name;
  func->as.function.return_type = parse_type(p);

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
    break;
  case AST_TYPE_UNKNOWN:
    printf("AstUnknownType: \n");
    break;
  case AST_TYPE_NAMED:
    printf("AstTypeNamed: (name: %s)\n", node->as.type_named.name);
    break;
  }
}
