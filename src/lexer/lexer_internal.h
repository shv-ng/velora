#pragma once

#include "lexer.h"

char advance(struct Lexer *l);
char peek(struct Lexer *l);
char peek_next(struct Lexer *l);

void skip_whitespace(struct Lexer *l);
struct Token make_tok(struct Lexer *l, enum TokenKind kind);

struct Token lexer_num(struct Lexer *l);
struct Token lexer_identifier(struct Lexer *l) ;
