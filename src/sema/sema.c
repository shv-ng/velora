#include "sema_internal.h"

#include <string.h>

struct SemaCtx sema_new(struct Parser *p) {
  struct SemaCtx sema;

  sema.error_count = 0;
  sema.current_return_type = &type_unknown;
  sema.current_scope = scope_new(p->arena, NULL);
  sema.file_name = p->lexer->file_name;
  sema.contents = p->lexer->contents;
  sema.arena = p->arena;

  return sema;
}

void sema_check(struct SemaCtx *sema, struct AstNode *root) {
  // collection of global decl
  for (int i = 0; i < root->as.program.count; i++) {
    struct AstNode *decl = root->as.program.declaration[i];
    struct Symbol *sym = symbol_new(sema->arena, decl);
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
    sema_node(sema, root->as.program.declaration[i], NULL);
  }
}

void sema_node(struct SemaCtx *sema, struct AstNode *node, struct Type *hint) {
  switch (node->kind) {
  case AST_FUNCTION_DECL:
    sema_func(sema, node);
    break;
  case AST_BLOCK_DECL:
    sema_block(sema, node, hint);
    break;
  case AST_RETURN_STMT:
    sema_return_stmt(sema, node);
    break;
  case AST_INT_LITERAL:
    node->resolved_type = hint ? hint : &type_unknown;
    break;
  case AST_TYPE_NAMED:
    node->resolved_type = resolve_type_node(node);
    break;
  default:
    break;
  }
}
