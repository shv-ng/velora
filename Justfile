LLVM_FLAGS_DEV    := `llvm-config --cflags --ldflags --libs core analysis native`
LLVM_FLAGS_STATIC := `llvm-config --cflags --ldflags --link-static --libs core analysis native`
VERSION           := `git describe --tags --always`
COMMIT            := `git rev-parse --short HEAD`
DATE              := `date +%Y-%m-%d`

STATIC_FLAGS := "-lpthread -lm -ldl -lstdc++ -static-libgcc -lz -lzstd"

# dev build with sanitizers
build-dev:
  @mkdir -p dist
  @clang ./src/*.c -o ./dist/velora \
    -Wall -Wextra -g -fsanitize=address \
    -DVERSION=\"{{VERSION}}\" \
    -DCOMMIT=\"{{COMMIT}}\" \
    -DDATE=\"{{DATE}}\" \
    {{LLVM_FLAGS_DEV}}

# release builds
build-linux-amd64:
  @mkdir -p dist
  @clang ./src/*.c -o ./dist/velora-linux-amd64 \
    -Wall -Wextra \
    -DVERSION=\"{{VERSION}}\" \
    -DCOMMIT=\"{{COMMIT}}\" \
    -DDATE=\"{{DATE}}\" \
    {{LLVM_FLAGS_STATIC}} \
    {{STATIC_FLAGS}}

build-linux-arm64:
  @mkdir -p dist
  @clang ./src/*.c -o ./dist/velora-linux-arm64 \
    -Wall -Wextra \
    -DVERSION=\"{{VERSION}}\" \
    -DCOMMIT=\"{{COMMIT}}\" \
    -DDATE=\"{{DATE}}\" \
    {{LLVM_FLAGS_STATIC}} \
    {{STATIC_FLAGS}}

build-darwin-arm64:
  @mkdir -p dist
  @clang ./src/*.c -o ./dist/velora-darwin-arm64 \
    -Wall -Wextra \
    -DVERSION=\"{{VERSION}}\" \
    -DCOMMIT=\"{{COMMIT}}\" \
    -DDATE=\"{{DATE}}\" \
    {{LLVM_FLAGS_STATIC}} \
    {{STATIC_FLAGS}}

# build all release targets
build: build-linux-amd64 build-linux-arm64 build-darwin-arm64

# run with dev build
run *args: build-dev
  @./dist/velora {{args}}

# clean
clean:
  @rm -rf dist
