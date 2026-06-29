
build:
  @clang ./file.c ./lexer.c main.c -o ./tmp/zyn -Wall -Wextra -g -fsanitize=address

run *args: build 
  @./tmp/zyn {{args}}
