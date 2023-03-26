#!/bin/bash

#######################################

cmake_build_test() {
    echo ==============================================================================
    echo $1
    echo ------------------------------------------------------------------------------
    cmake -B $1/build -S $1 -DCMAKE_BUILD_TYPE=Release -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
    echo ------------------------------------------------------------------------------
    cmake --build $1/build --config Release
    (
        echo ------------------------------------------------------------------------------
        cd $1/build
        ctest -C Release -V
    )
    echo ------------------------------------------------------------------------------
}

#######################################

cmake_build_test ./training/01_Task_Creation/1_xTaskCreateStatic
cmake_build_test ./training/01_Task_Creation/2_xTaskCreate

cmake_build_test ./training/02_Task_Control/1_vTaskDelay
cmake_build_test ./training/02_Task_Control/2_vTaskDelayUntil
cmake_build_test ./training/02_Task_Control/3_xTaskDelayUntil
cmake_build_test ./training/02_Task_Control/4_vTaskPrioritySet
cmake_build_test ./training/02_Task_Control/5_uxTaskPriorityGet

cmake_build_test ./training/03_Queue_Management/1_Queue_Send_And_Receive
