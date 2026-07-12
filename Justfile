
build:
  @mkdir -p tmp
  @clang ./src/*.c -o ./tmp/zyn -Wall -Wextra # -g -fsanitize=address

run *args: build 
  @./tmp/zyn {{args}}
