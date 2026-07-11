#include "sema.h"
#include "ast.h"
#include "error.h"
#include "symbol.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>

static struct Type *resolve_type_node(struct AstNode *node) {
  switch (node->kind) {
  case AST_TYPE_NAMED: {
    if (strcmp(node->as.type_named.name, "i32") == 0) {
      return &type_i32;
    }
  }
  default:
    return &type_unknown;
  }
}

static void sema_node(struct SemaCtx *sema, struct AstNode *node,
                      struct Type *hint);

struct SemaCtx *sema_new(char *file_name, char *contents) {
  struct SemaCtx *sema = malloc(sizeof(struct SemaCtx));

  sema->error_count = 0;
  sema->current_return_type = &type_unknown;
  sema->current_scope = scope_new(NULL);
  sema->file_name = file_name;
  sema->contents = contents;

  return sema;
}

static void sema_func(struct SemaCtx *sema, struct AstNode *node) {
  struct Type *prev_type = sema->current_return_type;

  sema->current_return_type = resolve_type_node(node->as.function.return_type);

  struct Symbol *sym =
      scope_lookup(sema->current_scope, node->as.function.name);

  if (sym == NULL) {
    sym = symbol_new(node);
    sym->type = node->resolved_type;
    sym->kind = SYMBOL_FUNC;
  }

  sema_node(sema, node->as.function.block, NULL);

  sema->current_return_type = prev_type;
}

static void sema_block(struct SemaCtx *sema, struct AstNode *node) {
  struct Scope *prev = sema->current_scope;

  sema->current_scope = scope_new(sema->current_scope);
  for (int i = 0; i < node->as.block.count; i++) {
    sema_node(sema, node->as.block.statements[i], NULL);
  }

  sema->current_scope = prev;
}

static void sema_return_stmt(struct SemaCtx *sema, struct AstNode *node) {
  sema_node(sema, node->as.return_stmt.expr, NULL);

  struct Type *actual_return_type =
      resolve_type_node(node->as.return_stmt.expr);

  if (!type_equal(actual_return_type, sema->current_return_type)) {
    struct Error err = {
        .kind = ERR_TYPE_MISMATCH,
        .span = node->span,
        .as.type_mismatch =
            {
                .expected = type_str(sema->current_return_type),
                .found = type_str(actual_return_type),
                .context = "return statement",
            },
    };
    print_error(err, sema->file_name, sema->contents);
  }
}

static void sema_node(struct SemaCtx *sema, struct AstNode *node,
                      struct Type *hint) {
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
    sema_node(sema, root->as.program.declaration[i], NULL);
  }
}
