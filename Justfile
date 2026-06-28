
build:
  @clang ./file.c ./lexer.c main.c -o ./tmp/zyn -Wall -Wextra

run *args: build 
  @./tmp/zyn {{args}}
