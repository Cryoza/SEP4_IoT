#include "gtest/gtest.h"
#include "../fff-master/fff.h"
DEFINE_FFF_GLOBALS;

extern "C" {
#include "Co2Sensor.h"
#include "mh_z19.h"
#include "shared_data.h"
}

FAKE_VALUE_FUNC( mh_z19_returnCode_t,mh_z19_takeMeassuring);

FAKE_VALUE_FUNC( mh_z19_returnCode_t, mh_z19_getCo2Ppm,uint16_t*);

FAKE_VOID_FUNC(sd_setCo2, shared_data_t*, float);

FAKE_VALUE_FUNC(float, sd_getCo2, shared_data_t*);

class Co2TaskTest : public ::testing::Test {
protected:
	void SetUp() override {
		RESET_FAKE(mh_z19_takeMeassuring);
		RESET_FAKE(mh_z19_getCo2Ppm);
		RESET_FAKE(sd_getCo2);
		RESET_FAKE(sd_setCo2);
		FFF_RESET_HISTORY();
	}

	void TearDown() override {}
};

TEST(Co2Test, Test_methods_is_called) {
	mh_z19_takeMeassuring_fake.return_val = MHZ19_OK;
	getCO2FromSensor();

	ASSERT_EQ(1, mh_z19_takeMeassuring_fake.call_count);
	ASSERT_EQ(1, mh_z19_getCo2Ppm_fake.call_count);
	ASSERT_EQ(1, sd_setCo2_fake.call_count);
	ASSERT_EQ(1, sd_getCo2_fake.call_count);
}


/* getCo2Ppm -> setCo2 -> getCo2*/
TEST(Co2Test, Test_getCo2_from_sensor) {

	uint16_t _co2_sensor_ppm = 999.99;
	mh_z19_getCo2Ppm_fake.arg0_val = &_co2_sensor_ppm;
	getCO2FromSensor();

	ASSERT_EQ(0, sd_getCo2_fake.return_val);
}

