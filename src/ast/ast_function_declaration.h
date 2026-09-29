#pragma once

struct AstFunctionDeclaration {
  const char *name; // function name

  // for symbol lookup, func overloading etc.
  // add:fn (i32,i32) ->  add$i32$i32
  const char *name_key;

  struct AstNode *return_type;
  struct AstNode *block;
};
