#pragma once

#include "../error/error.h"
#include "sema.h"

void sema_node(struct SemaCtx *ctx, struct AstNode *node, struct Type *hint);
void sema_program(struct SemaCtx *ctx, struct AstNode *node);

void sema_function_declaration(struct SemaCtx *ctx, struct AstNode *node);
void sema_block_declaration(struct SemaCtx *ctx, struct AstNode *node, struct Type *hint);

void sema_return_statement(struct SemaCtx *ctx, struct AstNode *node);

void sema_binary_expression(struct SemaCtx *ctx, struct AstNode *node,
                      struct Type *hint);
void sema_unary_expression(struct SemaCtx *ctx, struct AstNode *node,
                     struct Type *hint);

void sema_variable_declaration(struct SemaCtx *ctx, struct AstNode *node);
void sema_identifier(struct SemaCtx *ctx, struct AstNode *node,
                     struct Type *hint);
void sema_assignment(struct SemaCtx *ctx, struct AstNode *node);

struct Type *sema_resolve_type_node(struct AstNode *node);

bool sema_check_type_resolve(struct SemaCtx *ctx, struct AstNode *node);
