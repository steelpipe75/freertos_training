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

#################################################################################

set(TestName "${ProjectName}_Test")

set(INCLUDE_DIR
    .
    ${RTOS_INCLUDE_DIR}
)

set(SOURCES
    ../sample_xTaskCreate.c
    ${RTOS_SOURCES}
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
