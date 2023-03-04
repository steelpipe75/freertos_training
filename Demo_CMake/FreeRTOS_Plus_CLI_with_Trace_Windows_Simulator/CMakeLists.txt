# 動作確認できたCMakeのバージョン
cmake_minimum_required(VERSION 3.16)

# project情報
set(ProjectName "FreeRTOS_Plus_CLI_with_Trace_Windows_Simulator")
project(${ProjectName}
        LANGUAGES C
)

# コンパイル情報をJSON形式で出力
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# in-source ビルドガード
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

#################################################################################

set(WORKSPACE_ROOT_DIR ../..)

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


set(FREERTOS_DIR        ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS)
set(FREERTOS_PLUS_DIR   ${WORKSPACE_ROOT_DIR}/lib/third_party/FreeRTOS/FreeRTOS-Plus)
set(DENOLIB_DIR         ${FREERTOS_PLUS_DIR}/VisualStudio_StaticProjects/FreeRTOS-Kernel)
set(DEMO_DIR            ${FREERTOS_PLUS_DIR}/Demo/FreeRTOS_Plus_CLI_with_Trace_Windows_Simulator)

include_directories(
    ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-CLI
    ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-Trace/Include
    ${FREERTOS_DIR}/Source/include
    ${FREERTOS_DIR}/Source/portable/MSVC-MingW
    ${DEMO_DIR}/../../Common/include
    ${DEMO_DIR}/Trace_Recorder_Configuration
    ${DENOLIB_DIR}
    ${DEMO_DIR}
)

add_executable(${ProjectName}
    ${FREERTOS_DIR}/Source/list.c
    ${FREERTOS_DIR}/Source/portable/MemMang/heap_3.c
    ${FREERTOS_DIR}/Source/portable/MSVC-MingW/port.c
    ${FREERTOS_DIR}/Source/queue.c
    ${FREERTOS_DIR}/Source/tasks.c
    ${FREERTOS_DIR}/Source/timers.c
    ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-CLI/FreeRTOS_CLI.c
    ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-Trace/trcKernelPort.c
    ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-Trace/trcSnapshotRecorder.c
    ${FREERTOS_PLUS_DIR}/Source/FreeRTOS-Plus-Trace/trcStreamingRecorder.c
    ${DENOLIB_DIR}/freertos_hooks_winsim.c
    ${DEMO_DIR}/CLI-commands.c
    ${DEMO_DIR}/main.c
    ${DEMO_DIR}/Run-time-stats-utils.c
    ${DEMO_DIR}/UDPCommandServer.c
)

target_link_libraries(${ProjectName} winmm ws2_32)
