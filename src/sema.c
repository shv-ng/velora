#include "sema.h"
#include "ast.h"
#include "symbol.h"
#include "types.h"
#include <stdlib.h>

static void sema_node(struct SemaCtx *sema, struct AstNode *node);

struct SemaCtx *sema_new(void) {
  struct SemaCtx *sema = malloc(sizeof(struct SemaCtx));

  sema->error_count = 0;
  sema->current_return_type = &type_unknown;
  sema->current_scope = scope_new(NULL);

  return sema;
}

static void sema_func(struct SemaCtx *sema, struct AstNode *node) {
  struct Type *prev_type = sema->current_return_type;

  struct AstNode *return_type = node->as.function.return_type;
  sema->current_return_type = return_type->resolved_type;

  struct Symbol *sym = symbol_new(node);
  sym->type = node->resolved_type;
  sym->kind = SYMBOL_FUNC;

  sema_node(sema, node->as.function.block);

  sema->current_return_type = prev_type;
}

static void sema_block(struct SemaCtx *sema, struct AstNode *node) {
  struct Scope *prev = sema->current_scope;
  sema->current_scope = scope_new(sema->current_scope);

  for (int i = 0; i < node->as.block.count; i++) {
    sema_node(sema, node->as.block.statements[i]);
  }

  sema->current_scope = prev;
}

static void sema_return_stmt(struct SemaCtx *sema, struct AstNode *node) {
  sema_node(sema, node->as.return_stmt.expr);
  sema->current_return_type 
}

static void sema_node(struct SemaCtx *sema, struct AstNode *node) {
  switch (node->kind) {
  case AST_FUNCTION_DECL:
    sema_func(sema, node);
    break;
  case AST_BLOCK_DECL:
    sema_block(sema, node);
    break;
  case AST_RETURN_STMT:
    sema_return_stmt(sema, node);
    break;
  case AST_INT_LITERAL:
    node->resolved_type = &type_i32;
    break;
  default:
    break;
  }
}

void sema_check(struct SemaCtx *sema, struct AstNode *root) {
  // collection of global decl
  for (int i = 0; i < root->as.program.count; i++) {
    struct AstNode *decl = root->as.program.declaration[i];
    struct Symbol *sym = symbol_new(decl);
    switch (decl->kind) {
    case AST_FUNCTION_DECL:
      sym->kind = SYMBOL_FUNC;
      scope_define(sema->current_scope, decl->as.function.name, sym);
      break;
    default:
      break;
    }
  }

  // recursive semantic analysis+type checking
  for (int i = 0; i < root->as.program.count; i++) {
    sema_node(sema, root->as.program.declaration[i]);
  }
}
