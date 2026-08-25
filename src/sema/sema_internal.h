#pragma once

#include "../error/error.h"
#include "sema.h"

void sema_node(struct SemaCtx *sema, struct AstNode *node, struct Type *hint);

void sema_func(struct SemaCtx *sema, struct AstNode *node);
void sema_block(struct SemaCtx *sema, struct AstNode *node);
void sema_return_stmt(struct SemaCtx *sema, struct AstNode *node);

struct Type *resolve_type_node(struct AstNode *node);
