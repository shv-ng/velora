
build:
  @mkdir -p tmp
  @clang   ./src/*.c -o ./tmp/velora -Wall -Wextra # -g -fsanitize=address

run *args: build 
  @./tmp/velora {{args}}
