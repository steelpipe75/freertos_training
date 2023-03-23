#include <thread>
#include "gtest/gtest.h"

extern "C" {
	#include "sample_Queue_Send_And_Receive.h"
}

/*-----------------------------------------------------------*/

namespace {

#if defined(__unix) || defined(__unix__)
	const long long WaitTime = 500;
#else /* #if defined(__unix) || defined(__unix__) */
	const long long WaitTime = 500;
#endif /* #if defined(__unix) || defined(__unix__) */

static void drive_freertos(void);

static void drive_freertos(void){
	sample_Queue_Send_And_Receive();
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
	std::this_thread::sleep_for(std::chrono::milliseconds(WaitTime));
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	int ret =  RUN_ALL_TESTS();
	return ret;
}

}	// namespace
