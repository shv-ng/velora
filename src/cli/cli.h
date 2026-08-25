#pragma once

#include "../ast/ast.h"
#include "../codegen/codegen.h"
#include "../file/file.h"
#include "../lexer/lexer.h"
#include "../sema/sema.h"
#include "../utils/arena.h"

int cli_new(int argc, char *argv[]);
