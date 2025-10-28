#include <unity.h>

static void simpleAssertionSuccess()
{
    TEST_ASSERT(4 == 4);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(simpleAssertionSuccess);
    return UNITY_END();
}