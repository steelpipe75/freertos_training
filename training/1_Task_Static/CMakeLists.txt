# 動作確認できたCMakeのバージョン
cmake_minimum_required(VERSION 3.16)

# project情報
set(ProjectName "build_test")
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

set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "Coverage")
endif()

set(CMAKE_C_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_COVERAGE "-g -O0 -fprofile-instr-generate -fcoverage-mapping")

#################################################################################

include_directories(
    ../../lib/third_party/FreeRTOS/FreeRTOS/Source/include
    ../../lib/third_party/FreeRTOS/FreeRTOS/Demo/Common/include
    ../../lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix
    ../../lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix/utils
    .
)

add_executable(
    freertos_training_1_task_s
    
    main.c
    ../../lib/third_party/FreeRTOS/FreeRTOS/Source/event_groups.c
    ../../lib/third_party/FreeRTOS/FreeRTOS/Source/stream_buffer.c
    ../../lib/third_party/FreeRTOS/FreeRTOS/Source/timers.c
    ../../lib/third_party/FreeRTOS/FreeRTOS/Source/list.c
    ../../lib/third_party/FreeRTOS/FreeRTOS/Source/queue.c
    ../../lib/third_party/FreeRTOS/FreeRTOS/Source/tasks.c
    ../../lib/third_party/FreeRTOS/FreeRTOS/Demo/Common/Minimal/StaticAllocation.c
    ../../lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix/port.c
    ../../lib/third_party/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c
)
target_link_libraries(freertos_training_1_task_s pthread)
