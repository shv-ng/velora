
build:
  @mkdir -p tmp
  @clang   ./src/*.c -o ./tmp/velora -Wall -Wextra \
          -DVERSION=\"$(git describe --tags --always)\" \
          -DCOMMIT=\"$(git rev-parse --short HEAD)\" \
          -DDATE=\"$(date +%Y-%m-%d)\" # -g -fsanitize=address

run *args: build 
  @./tmp/velora {{args}}
