CC = clang
FLAGS = -Wall -Wextra  -g -fsanitize=address,undefined 
SRC = $(wildcard src/*.c src/*/*.c)

dist/velora: $(SRC)
	@mkdir -p dist
	@$(CC) $(SRC) -o dist/velora $(FLAGS)\
		-DVERSION=\"$(shell git describe --tags --always)\" \
		-DCOMMIT=\"$(shell git rev-parse --short HEAD)\" \
		-DDATE=\"$(shell date +%Y-%m-%d)\" \
		$(shell llvm-config --cflags --ldflags --libs core analysis)

run: dist/velora 
	@./dist/velora run $(filter-out $@, $(MAKECMDGOALS))

format:
	@clang-format -i src/**/*.c src/**/*.h 

.PHONY: run clean
