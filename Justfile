
build:
  @mkdir -p tmp
  @clang ./src/file.c ./src/lexer.c ./src/main.c -o ./tmp/zyn -Wall -Wextra -g -fsanitize=address

run *args: build 
  @./tmp/zyn {{args}}
