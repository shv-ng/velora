
#include <assert.h>


int add(int a, int b) { return a + b; }

void test_add() {
  assert(add(1, 4) == 5);
  assert(add(2, 5) == 7);
}

