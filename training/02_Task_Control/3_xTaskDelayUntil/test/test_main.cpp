#include <thread>
#include "gtest/gtest.h"

extern "C" {
	#include "sample_xTaskDelayUntil.h"
}

/*-----------------------------------------------------------*/

namespace {
	
static void drive_freertos(void);

static void drive_freertos(void){
	sample_xTaskDelayUntil();
	FAIL();
}

class FreeRtosUnitTest : public testing::Test {
protected:
	void SetUp() override {
		std::thread th_freertos(drive_freertos);
		th_freertos.detach();
	}
	void TearDown() override {
	}
};

TEST_F(FreeRtosUnitTest, Sample1) {
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	int ret =  RUN_ALL_TESTS();
	return ret;
}

}	// namespace
