#pragma once

#include "../ast/ast.h"
#include "parser.h"

void parser_advance(struct ParserCtx *ctx);
void parser_synchronise(struct ParserCtx *ctx);
void parser_expect(struct ParserCtx *ctx, enum TokenKind kind);

// need to add span for sure
struct AstNode *astnode_new(struct ParserCtx *ctx, enum AstKind kind);

struct AstNode *parse_declaration(struct ParserCtx *ctx);
struct AstNode *parse_function_declaration(struct ParserCtx *ctx, char *name);
struct AstNode *parse_variable_declaration(struct ParserCtx *ctx, char *name,
                                           struct AstNode *type,
                                           struct Span start);

struct AstNode *parse_expression(struct ParserCtx *ctx, int min_bp);

struct AstNode *parse_statement(struct ParserCtx *ctx, struct AstNode *block);

struct AstNode *parse_return_statement(struct ParserCtx *ctx);

struct AstNode *parse_if_else_expression(struct ParserCtx *ctx);
struct AstNode *parse_block_declaration(struct ParserCtx *ctx, char *name);

struct AstNode *parse_type(struct ParserCtx *ctx);

struct AstNode *parse_primary(struct ParserCtx *ctx);

struct Token comment_merge(struct ParserCtx *ctx, struct Token comment1,
                           struct Token comment2);
