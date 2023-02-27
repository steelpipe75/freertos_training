# 動作確認できたCMakeのバージョン
cmake_minimum_required(VERSION 3.16)

# project情報
set(ProjectName "freertos_training_xTaskDelayUntil")
project(${ProjectName}
        LANGUAGES C CXX
)

# コンパイル情報をJSON形式で出力
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# in-source ビルドガード
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

#################################################################################

set(WORKSPACE_ROOT_DIR ../../..)

#################################################################################

set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "Coverage")
endif()

if (MSVC)
    set(CMAKE_C_FLAGS "/W4 /WX-")
else()
    set(CMAKE_C_FLAGS "-Wall -Wextra")
    set(CMAKE_C_FLAGS_DEBUG "-g -O0")
    set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
    set(CMAKE_C_FLAGS_COVERAGE "-g -O0 -fprofile-instr-generate -fcoverage-mapping")
endif()

#################################################################################

option(USE_MY_TRACE "USE_MY_TRACE" ON)

#################################################################################

include(${WORKSPACE_ROOT_DIR}/lib/RTOS/RTOS.cmake)
if(USE_MY_TRACE)
    include(${WORKSPACE_ROOT_DIR}/lib/RTOS/MyTrace/MyTrace.cmake)
    add_definitions(-DUSE_MY_TRACE)
endif()

#################################################################################

set(INCLUDE_DIR
    .
    ${WORKSPACE_ROOT_DIR}/lib/other
    ${RTOS_INCLUDE_DIR}
    ${RTOS_MY_TRACE_INCLUDE_DIR}
)

set(SOURCES
    sample_xTaskDelayUntil.c
    ${WORKSPACE_ROOT_DIR}/lib/other/work.c
    ${RTOS_SOURCES}
    ${RTOS_MY_TRACE_SOURCES}
)

include_directories(
    ${INCLUDE_DIR}
)

add_executable(${ProjectName}
    ./main.c
    ${SOURCES}
)

if(UNIX)
    target_link_libraries(${ProjectName} pthread)
elseif(WIN32)
    target_link_libraries(${ProjectName} winmm)
endif()

#################################################################################

option(BUILD_TESTS "BUILD_TESTS" ON)
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(./test)
endif()
