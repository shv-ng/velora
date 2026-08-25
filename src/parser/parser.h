#pragma once

#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "../token/token.h"

struct Parser {
  struct Lexer *lexer;
  struct Token current_token;
  struct Token next_token;
  struct Arena *arena;
  int error_count;
};

struct Parser parser_new(struct Lexer *l);

struct AstNode *parse_program(struct Parser *p);
