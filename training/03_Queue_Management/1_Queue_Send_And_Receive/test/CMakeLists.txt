# 動作確認できたCMakeのバージョン
cmake_minimum_required(VERSION 3.16)

#################################################################################

set(WORKSPACE_ROOT_DIR ../../../..)

#################################################################################

set(GTest_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${WORKSPACE_ROOT_DIR}/lib/third_party/googletest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${GTest_DIR} lib/third_party/googletest)
include(GoogleTest)

#################################################################################

if(USE_MY_TRACE)
    include(${WORKSPACE_ROOT_DIR}/lib/RTOS/MyTrace/MyTrace.cmake)
    add_definitions(-DUSE_MY_TRACE)
endif()

#################################################################################

set(TestName "${ProjectName}_Test")

set(INCLUDE_DIR
    .
    ${WORKSPACE_ROOT_DIR}/lib/other
    ${RTOS_MY_TRACE_INCLUDE_DIR}
)

set(SOURCES
    ../sample_Queue_Send_And_Receive.c
    ${WORKSPACE_ROOT_DIR}/lib/other/work.c
    ${RTOS_MY_TRACE_SOURCES}
)

include_directories(
    ${INCLUDE_DIR}
)

add_executable(${TestName}
    ./test_main.cpp
    ${SOURCES}
)

if(UNIX)
    target_link_libraries(${TestName} pthread)
elseif(WIN32)
    target_link_libraries(${TestName} winmm)
endif()

target_link_libraries(${TestName} gtest_main FreeRTOS)

gtest_discover_tests(${TestName})
