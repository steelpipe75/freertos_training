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

include(${WORKSPACE_ROOT_DIR}/lib/RTOS/RTOS.cmake)
include(${WORKSPACE_ROOT_DIR}/lib/RTOS/MyTrace/MyTrace.cmake)

#################################################################################

set(TestName "${ProjectName}_Test")

set(INCLUDE_DIR
    .
    ${WORKSPACE_ROOT_DIR}/lib/other
    ${RTOS_INCLUDE_DIR}
    ${RTOS_MY_TRACE_INCLUDE_DIR}
)

set(SOURCES
    ../sample_vTaskDelayUntil.c
    ${WORKSPACE_ROOT_DIR}/lib/other/work.c
    ${RTOS_SOURCES}
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
    target_link_libraries(${TestName} pthread gtest_main)
elseif(WIN32)
    target_link_libraries(${TestName} winmm gtest_main)
endif()

gtest_discover_tests(${TestName})
