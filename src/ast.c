
#include "ast.h"
#include "error.h"
#include "lexer.h"
#include "types.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static struct AstNode *astnode_new(enum AstKind kind) {
  struct AstNode *node = malloc(sizeof(struct AstNode));
  node->resolved_type = &type_unknown;
  node->kind = kind;
  return node;
}

static void advance(struct Parser *p) {
  p->current_token = p->next_token;
  p->next_token = next_token(p->lexer);
}

static void synchronise(struct Parser *p) {
  // dumber error recovery

  while (p->current_token.kind != TOK_EOF) {
    switch (p->current_token.kind) {
    case TOK_EOF:
    case TOK_SEMICOLON:
    case TOK_LBRACE:
    case TOK_RBRACE:
      return;
    default:
      advance(p);
    }
  }
}
static void expect(struct Parser *p, enum TokenKind kind) {
  if (p->current_token.kind == kind) {
    advance(p);
    return;
  }

  struct Error err = {
      .kind = ERR_SYNTAX,
      .span = p->current_token.span,
      .as.syntax.expected = token_kind_str(kind),
      .as.syntax.found = token_kind_str(p->current_token.kind),
  };
  print_error(err, p->lexer->file_name, p->lexer->contents);

  p->error_count++;

  return;
}

static struct AstNode *parse_expr(struct Parser *p) {
  switch (p->current_token.kind) {
  case TOK_INT_LITERAL: {
    struct Token int_tok = p->current_token;
    expect(p, TOK_INT_LITERAL);

    struct AstNode *node = astnode_new(AST_INT_LITERAL);
    node->as.int_literal.value = atoll(int_tok.val);
    node->span = int_tok.span;

    return node;
  }
  default: {
    struct Error err = {
        .kind = ERR_SYNTAX,
        .span = p->current_token.span,
        .as.syntax.expected = "expression",
        .as.syntax.found = token_kind_str(p->current_token.kind),
    };

    print_error(err, p->lexer->file_name, p->lexer->contents);
    synchronise(p);
    p->error_count++;
    return NULL;
  }
  }
}

static struct AstNode *parse_return_stmt(struct Parser *p) {
  struct Span start = p->current_token.span;
  expect(p, TOK_KW_RETURN);

  struct AstNode *return_stmt = astnode_new(AST_RETURN_STMT);

  if (p->current_token.kind != TOK_SEMICOLON) {
    return_stmt->as.return_stmt.expr = parse_expr(p);
  }

  struct Span end = p->current_token.span;
  expect(p, TOK_SEMICOLON);
  return_stmt->span = merge_span(start, end);

  return return_stmt;
}

static struct AstNode *parse_block(struct Parser *p, char *name) {
  struct Span start = p->current_token.span;
  expect(p, TOK_LBRACE);

  int capacity = 10;
  struct AstNode **statements = malloc(sizeof(struct AstNode *) * capacity);
  int count = 0;

  while (p->current_token.kind != TOK_EOF &&
         p->current_token.kind != TOK_RBRACE) {
    if (count >= capacity) {
      capacity *= 2;
      statements = realloc(statements, sizeof(struct AstNode *) * capacity);
    }
    struct AstNode *stmt = NULL;

    switch (p->current_token.kind) {
    case TOK_KW_RETURN:
      stmt = parse_return_stmt(p);
      break;
    default: {
      struct Error err = {
          .kind = ERR_SYNTAX,
          .span = p->current_token.span,
          .as.syntax.found = token_kind_str(p->current_token.kind),
      };
      print_error(err, p->lexer->file_name, p->lexer->contents);
      p->error_count++;

      synchronise(p);
      continue;
    }
    }
    if (stmt != NULL) {
      statements[count++] = stmt;
    }
  }

  struct AstNode *block = astnode_new(AST_BLOCK_DECL);

  block->as.block.name = name;
  block->as.block.statements = statements;
  block->as.block.count = count;

  expect(p, TOK_RBRACE);
  struct Span end = p->current_token.span;
  block->span = merge_span(start, end);
  return block;
}

static struct AstNode *parse_type(struct Parser *p) {
  struct Span start = p->current_token.span;
  struct AstNode *type = astnode_new(AST_TYPE_UNKNOWN);

  if (p->current_token.kind == TOK_IDENTIFIER) {
    struct Token name_tok = p->current_token;
    expect(p, TOK_IDENTIFIER);

    type->kind = AST_TYPE_NAMED;
    type->as.type_named.name = name_tok.val;
  }

  if (type->kind == AST_TYPE_UNKNOWN) {
    struct Error err = {
        .kind = ERR_SYNTAX,
        .span = p->current_token.span,
        .as.syntax.found = token_kind_str(p->current_token.kind),
    };
    print_error(err, p->lexer->file_name, p->lexer->contents);
    advance(p);
    p->error_count++;
  }
  struct Span end = p->current_token.span;
  type->span = merge_span(start, end);
  return type;
}

static struct AstNode *parse_func_decl(struct Parser *p, char *name) {
  struct Span start = p->current_token.span;
  expect(p, TOK_KW_FUNC);

  expect(p, TOK_LPAREN);
  expect(p, TOK_RPAREN);

  struct AstNode *return_type = parse_type(p);
  struct AstNode *block = parse_block(p, name);

  if (p->error_count > 0) {
    return NULL;
  }

  struct AstNode *func = astnode_new(AST_FUNCTION_DECL);

  func->as.function.name = name;
  func->as.function.return_type = return_type;
  func->as.function.block = block;

  struct Span end = p->current_token.span;
  func->span = merge_span(start, end);
  return func;
}

static struct AstNode *parse_declaration(struct Parser *p) {
  struct Token name_tok = p->current_token;
  expect(p, TOK_IDENTIFIER);
  expect(p, TOK_COLON);

  if (p->current_token.kind == TOK_KW_FUNC) {
    return parse_func_decl(p, name_tok.val);
  }
  return NULL;
}

struct AstNode *parse_program(struct Parser *p) {
  int capacity = 10;
  int count = 0;

  struct AstNode **declaration = malloc(sizeof(struct AstNode *) * capacity);

  while (p->current_token.kind != TOK_EOF) {
    if (count >= capacity) {
      capacity *= 2;
      declaration = realloc(declaration, sizeof(struct AstNode *) * capacity);
    }
    if (p->current_token.kind == TOK_IDENTIFIER) {
      struct AstNode *decl = parse_declaration(p);
      if (decl != NULL) {
        declaration[count++] = decl;
      }

    } else {
      struct Error err = {
          .kind = ERR_SYNTAX,
          .span = p->current_token.span,
          .as.syntax.found = token_kind_str(p->current_token.kind),
      };
      print_error(err, p->lexer->file_name, p->lexer->contents);
      p->error_count++;

      advance(p);
    }
  }

  struct AstNode *program = astnode_new(AST_PROGRAM);
  program->as.program.count = count;
  program->as.program.declaration = declaration;

  return program;
}

struct Parser parser_new(struct Lexer *l) {
  struct Parser p = (struct Parser){
      .lexer = l,
      .error_count = 0,
  };

  p.current_token = next_token(l);
  p.next_token = next_token(l);

  return p;
}

static void indentation(int indent) {
  for (int i = 0; i < indent; i++)
    printf("  ");
}

void print_ast(struct AstNode *node, int indent) {
  if (node == NULL)
    return;

  indentation(indent);

  switch (node->kind) {
  case AST_PROGRAM: {
    printf("AstProgram: (resolved_type: %s)\n", type_str(node->resolved_type));
    for (int i = 0; i < node->as.program.count; i++) {
      print_ast(node->as.program.declaration[i], indent + 1);
    }
    break;
  }
  case AST_FUNCTION_DECL:
    printf("AstFunctionDecl: (name: %s, resolved_type: %s)\n",
           node->as.function.name, type_str(node->resolved_type));
    print_ast(node->as.function.return_type, indent + 1);
    print_ast(node->as.function.block, indent + 1);
    break;
  case AST_TYPE_UNKNOWN:
    printf("AstUnknownType: \n");
    break;
  case AST_TYPE_NAMED:
    printf("AstTypeNamed: (name: %s, resolved_type: %s)\n",
           node->as.type_named.name, type_str(node->resolved_type));
    break;
  case AST_BLOCK_DECL:
    printf("AstBlockDecl: (name: %s, resolved_type: %s)\n", node->as.block.name,
           type_str(node->resolved_type));
    for (int i = 0; i < node->as.block.count; i++) {
      print_ast(node->as.block.statements[i], indent + 1);
    }
    break;
  case AST_RETURN_STMT:
    printf("AstReturnStmt: (resolved_type: %s)\n",
           type_str(node->resolved_type));
    print_ast(node->as.return_stmt.expr, indent + 1);
    break;
  case AST_INT_LITERAL:
    printf("AstIntLiteral: (value: %lld, resolved_type: %s)\n",
           node->as.int_literal.value, type_str(node->resolved_type));
    break;
  }
}
