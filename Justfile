build:
  @gcc main.c -o ./tmp/zyn

run *args: build 
  @./tmp/zyn {{args}}

test:
  @./test_generator.sh
  @./test_runner.sh
