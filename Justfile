build:
  @mkdir -p dist
  @clang  ./src/*.c ./src/*/*.c -o ./dist/velora -Wall -Wextra  -g -fsanitize=address,undefined \
          -DVERSION=\"$(git describe --tags --always)\" \
          -DCOMMIT=\"$(git rev-parse --short HEAD)\" \
          -DDATE=\"$(date +%Y-%m-%d)\" \
          $(llvm-config --cflags --ldflags --libs core analysis)

run *args: build 
  @./dist/velora {{args}}
