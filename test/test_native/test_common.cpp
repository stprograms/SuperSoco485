#include <unity.h>
#include "test_super_soco.h"


int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_super_soco_init_no_param);
    return UNITY_END();
}
