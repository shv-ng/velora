#!/usr/bin/env bash

set -e 

RUNNER_FILE="./tmp/generated_runner.c"

echo '#include <stdio.h>' > "$RUNNER_FILE"
echo '#include <assert.h>' >> "$RUNNER_FILE"

TEST_FUNCS=$(fd -t f --glob "*_test.c" --exec sh -c "cat {}" | rg "test_\w*\(\)" -o | sed 's/()//')

for func in $TEST_FUNCS; do
    echo "void ${func}(void);" >> "$RUNNER_FILE"
done

echo 'int main() {' >> "$RUNNER_FILE"
for func in $TEST_FUNCS; do
    echo "    printf(\"Running %s...\\n\", \"$func\");" >> "$RUNNER_FILE"
    echo "    ${func}();" >> "$RUNNER_FILE"
done

echo '    printf("All tests passed successfully!\n");' >> "$RUNNER_FILE"
echo '    return 0;' >> "$RUNNER_FILE"
echo '}' >> "$RUNNER_FILE"

