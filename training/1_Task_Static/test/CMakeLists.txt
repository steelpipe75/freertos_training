# 動作確認できたCMakeのバージョン
cmake_minimum_required(VERSION 3.16)

#################################################################################

set(GTest_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/third_party/googletest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${GTest_DIR} lib/third_party/googletest)
include(GoogleTest)

#################################################################################

set(TestName "${ProjectName}_Test")

include_directories(
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/include
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Demo/Common/include
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix/utils
    ../
)

add_executable(${TestName}
    test_main.cpp
    ../TaskCreateStatic.c
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/event_groups.c
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/stream_buffer.c
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/timers.c
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/list.c
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/queue.c
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/tasks.c
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Demo/Common/Minimal/StaticAllocation.c
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix/port.c
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c
)
target_link_libraries(${TestName} pthread gtest_main)
gtest_discover_tests(${TestName})
