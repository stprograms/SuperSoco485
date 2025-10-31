#include <unity.h>
#include <stdio.h>
#include "SuperSoco485.h"

using namespace stprograms::SuperSoco485;

class MockSuperSoco485 : public SuperSoco485
{
public:
    MockSuperSoco485() : SuperSoco485() {}

    DataChangedHandler getVehicleDataUpdatedHandler() const
    {
        return vehicleDataUpdatedHandler;
    }

    void *getUserData() const
    {
        return _user_data;
    }
};

void test_super_soco_init_no_param()
{
    MockSuperSoco485 ss;
    ss.begin();

    TEST_ASSERT_NULL(ss.getVehicleDataUpdatedHandler());
    TEST_ASSERT_NULL(ss.getUserData());
}
