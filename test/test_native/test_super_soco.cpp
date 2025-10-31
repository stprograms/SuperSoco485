#include <unity.h>
#include "SuperSoco485.h"

using namespace stprograms::SuperSoco485;

class MockSuperSoco485 : public SuperSoco485
{
public:
    DataChangedHandler getVehicleDataUpdatedHandler() const
    {
        return vehicleDataUpdatedHandler;
    }
};



void test_super_soco_init_no_param()
{
    MockSuperSoco485 ss;
    ss.begin();

    TEST_ASSERT(ss.getVehicleDataUpdatedHandler() == NULL);
}



// int main()
// {
//     // put your setup code here, to run once:
//     UNITY_BEGIN();
//     RUN_TEST(test_init);
//     UNITY_END();
// }
