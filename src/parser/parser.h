#pragma once

#include "../lexer/lexer.h"
#include "../token/token.h"
#include "../utils/arena.h"

struct ParserCtx {
  struct LexerCtx *lexer;
  struct Token current_token;
  struct Token next_token;
  struct Arena *arena;
  int error_count;
};

struct ParserCtx parser_new(struct LexerCtx *lexer_ctx);

struct AstNode *parse_program(struct ParserCtx *ctx);
