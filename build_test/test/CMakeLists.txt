# 動作確認できたCMakeのバージョン
cmake_minimum_required(VERSION 3.16)

set(GTest_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/third_party/googletest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${GTest_DIR} lib/third_party/googletest)
include(GoogleTest)

add_subdirectory(./main)
