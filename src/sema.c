#include "sema.h"
#include "ast.h"
#include "error.h"
#include "symbol.h"
#include "types.h"
#include <string.h>

static struct Type *resolve_type_node(struct AstNode *node) {
  struct Type *t = &type_unknown;
  switch (node->kind) {
  case AST_TYPE_NAMED: {
    if (strcmp(node->as.type_named.name, "i32") == 0) {
      t = &type_i32;
    } else if (strcmp(node->as.type_named.name, "i8") == 0) {
      t = &type_i8;
    }
    break;
  }
  default:
    break;
  }
  return t;
}

static void sema_node(struct SemaCtx *sema, struct AstNode *node,
                      struct Type *hint);

struct SemaCtx sema_new(struct Parser *p) {
  struct SemaCtx sema;

  sema.error_count = 0;
  sema.current_return_type = &type_unknown;
  sema.current_scope = scope_new(NULL);
  sema.file_name = p->lexer->file_name;
  sema.contents = p->lexer->contents;

  return sema;
}

static void sema_func(struct SemaCtx *sema, struct AstNode *node) {
  struct Type *prev_type = sema->current_return_type;

  sema_node(sema, node->as.function.return_type, NULL);

  sema->current_return_type = resolve_type_node(node->as.function.return_type);
  node->resolved_type = sema->current_return_type;

  sema_node(sema, node->as.function.block, NULL);

  sema->current_return_type = prev_type;
}

static void sema_block(struct SemaCtx *sema, struct AstNode *node) {
  struct Scope *prev = sema->current_scope;

  sema->current_scope = scope_new(sema->current_scope);

  node->resolved_type = &type_void;
  for (int i = 0; i < node->as.block.count; i++) {
    sema_node(sema, node->as.block.statements[i], NULL);
  }

  if (node->as.block.count > 0) {
    node->resolved_type =
        node->as.block.statements[node->as.block.count - 1]->resolved_type;
  }


  sema->current_scope = prev;
}

static void sema_return_stmt(struct SemaCtx *sema, struct AstNode *node) {
  sema_node(sema, node->as.return_stmt.expr, sema->current_return_type);

  struct Type *actual = node->as.return_stmt.expr->resolved_type;
  node->resolved_type = actual;

  if (!type_equal(actual, sema->current_return_type)) {
    struct Error err = {
        .kind = ERR_TYPE_MISMATCH,
        .span = node->span,
        .as.type_mismatch =
            {
                .expected = type_str(sema->current_return_type),
                .found = type_str(actual),
                .context = "return statement",
            },
    };
    sema->error_count++;
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
    node->resolved_type = hint ? hint : &type_unknown;
    break;
  case AST_TYPE_NAMED:
    node->resolved_type = resolve_type_node(node);
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
