#include "../src/fsm.h"
#include "../lib/unity/unity.h"

void setUp() {}

void tearDown() {}

void sumTest() {
    struct fsm *fsm;
    TEST_ASSERT_FALSE(verify(fsm));
}

int main() {
   UNITY_BEGIN();
   RUN_TEST(sumTest);
   UNITY_END();
}
