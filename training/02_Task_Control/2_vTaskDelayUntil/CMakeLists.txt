# 動作確認できたCMakeのバージョン
cmake_minimum_required(VERSION 3.16)

# project情報
set(ProjectName "freertos_training_vTaskDelayUntil")
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

set(INCLUDE_DIR
    .
    ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/include
    ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Demo/Common/include
)

set(SOURCES
    sample_vTaskDelayUntil.c
    ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/event_groups.c
    ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/stream_buffer.c
    ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/timers.c
    ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/list.c
    ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/queue.c
    ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/tasks.c
    ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/portable/MemMang/heap_3.c
)

if(UNIX)
    set(INCLUDE_DIR
        ${INCLUDE_DIR}
        ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix
        ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix/utils
    )

    set(SOURCES
        ${SOURCES}
        ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix/port.c
        ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c
    )
elseif(WIN32)
    set(INCLUDE_DIR
        ${INCLUDE_DIR}
        ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/portable/MSVC-MingW
    )

    set(SOURCES
        ${SOURCES}
        ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS/Source/portable/MSVC-MingW/port.c
    )
endif()

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
