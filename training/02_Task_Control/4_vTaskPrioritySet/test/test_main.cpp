#include <thread>
#include "gtest/gtest.h"

extern "C" {
	#include "sample_vTaskPrioritySet.h"
}

/*-----------------------------------------------------------*/

namespace {

#if defined(__unix) || defined(__unix__)
	const long long WaitTime = 1;
#else /* #if defined(__unix) || defined(__unix__) */
	const long long WaitTime = 1;
#endif /* #if defined(__unix) || defined(__unix__) */

static void drive_freertos(void);

static void drive_freertos(void){
	sample_vTaskPrioritySet();
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
	std::this_thread::sleep_for(std::chrono::seconds(WaitTime));
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	int ret =  RUN_ALL_TESTS();
	return ret;
}

}	// namespace
