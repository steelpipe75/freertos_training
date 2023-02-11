# 動作確認できたCMakeのバージョン
cmake_minimum_required(VERSION 3.16)

#################################################################################

set(GTest_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/third_party/googletest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${GTest_DIR} lib/third_party/googletest)
include(GoogleTest)

#################################################################################

set(TestName "${ProjectName}_Test")

set(INCLUDE_DIR
    .
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/include
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Demo/Common/include
)

set(SOURCES
    ../sample_xTaskCreateStatic.c
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/event_groups.c
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/stream_buffer.c
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/timers.c
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/list.c
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/queue.c
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/tasks.c
    ../../../lib/third_party/FreeRTOS/FreeRTOS/Demo/Common/Minimal/StaticAllocation.c
)

if(UNIX)
    set(INCLUDE_DIR
        ${INCLUDE_DIR}
        ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix
        ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix/utils
    )

    set(SOURCES
        ${SOURCES}
        ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix/port.c
        ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c
    )
elseif(WIN32)
    set(INCLUDE_DIR
        ${INCLUDE_DIR}
        ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/portable/MSVC-MingW
    )

    set(SOURCES
        ${SOURCES}
        ../../../lib/third_party/FreeRTOS/FreeRTOS/Source/portable/MSVC-MingW/port.c
    )
endif()

include_directories(
    ${INCLUDE_DIR}
)

add_executable(${TestName}
    ./test_main.cpp
    ${SOURCES}
)

if(UNIX)
    target_link_libraries(${TestName} pthread gtest_main)
elseif(WIN32)
    target_link_libraries(${TestName} winmm gtest_main)
endif()

gtest_discover_tests(${TestName})
