#pragma once

#include "../utils/arena.h"
#include "../error/error.h"
#include "parser.h"

void advance(struct Parser *p);
void synchronise(struct Parser *p);
void expect(struct Parser *p, enum TokenKind kind);

struct AstNode *astnode_new(struct Parser *p, enum AstKind kind);

struct AstNode *parse_declaration(struct Parser *p);
struct AstNode *parse_func_decl(struct Parser *p, char *name);

struct AstNode *parse_expr(struct Parser *p);

struct AstNode *parse_return_stmt(struct Parser *p);
struct AstNode *parse_block(struct Parser *p, char *name);

struct AstNode *parse_type(struct Parser *p);
