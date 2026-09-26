#pragma once

#include "../token/token.h"
#include "../utils/arena.h"
#include "lexer.h"

char lexer_advance(struct LexerCtx *ctx);
char lexer_peek(struct LexerCtx *ctx);
char lexer_peek_next(struct LexerCtx *ctx);

void lexer_skip_whitespace(struct LexerCtx *ctx);
struct Token lexer_make_tok(struct LexerCtx *ctx, enum TokenKind kind);

struct Token lexer_number(struct LexerCtx *ctx);
struct Token lexer_identifier(struct LexerCtx *ctx);
