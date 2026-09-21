#pragma once

#include "../error/error.h"
#include "sema.h"

void sema_node(struct SemaCtx *ctx, struct AstNode *node, struct Type *hint);

void sema_func(struct SemaCtx *ctx, struct AstNode *node);
void sema_block(struct SemaCtx *ctx, struct AstNode *node, struct Type *hint);
void sema_return_stmt(struct SemaCtx *ctx, struct AstNode *node);
void sema_binary_expr(struct SemaCtx *ctx, struct AstNode *node,
                      struct Type *hint);
void sema_unary_expr(struct SemaCtx *ctx, struct AstNode *node,
                     struct Type *hint);

void sema_var_decl(struct SemaCtx *ctx, struct AstNode *node);
void sema_identifier(struct SemaCtx *ctx, struct AstNode *node,
                     struct Type *hint);

struct Type *resolve_type_node(struct AstNode *node);
