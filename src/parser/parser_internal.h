#pragma once

#include "../ast/ast.h"
#include "parser.h"

void parser_advance(struct ParserCtx *ctx);
void parser_synchronise(struct ParserCtx *ctx);
void parser_expect(struct ParserCtx *p, enum TokenKind kind);

struct AstNode *astnode_new(struct ParserCtx *p, enum AstKind kind);

struct AstNode *parse_declaration(struct ParserCtx *ctx);
struct AstNode *parse_function_declaration(struct ParserCtx *p, char *name);
struct AstNode *parse_variable_declaration(struct ParserCtx *p, char *name,
                                           struct AstNode *type,
                                           struct Span start);

struct AstNode *parse_expression(struct ParserCtx *p, int min_bp);

struct AstNode *parse_statement(struct ParserCtx *p, struct AstNode *block);

struct AstNode *parse_return_statement(struct ParserCtx *ctx);
struct AstNode *parse_block_declaration(struct ParserCtx *p, char *name);

struct AstNode *parse_type(struct ParserCtx *ctx);

struct AstNode *parse_primary(struct ParserCtx *ctx);
