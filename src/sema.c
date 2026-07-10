#include "sema.h"
#include "ast.h"
#include "symbol.h"
#include "types.h"
#include <stdlib.h>

struct SemaCtx *sema_new(void) {
  struct SemaCtx *sema = malloc(sizeof(struct SemaCtx));

  sema->error_count = 0;
  sema->current_return_type = &type_unknown;
  sema->current_scope = scope_new(NULL);

  return sema;
}

void sema_check(struct SemaCtx *sema, struct AstNode *root) {
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
}
