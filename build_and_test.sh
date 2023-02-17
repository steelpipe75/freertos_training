#!/bin/bash

#######################################

cmake_build_test() {
    echo $1
    cmake -B $1/build -S $1 -DCMAKE_BUILD_TYPE=Release -G Ninja
    cmake --build $1/build --config Release
    (
        cd $1/build
        ctest -C Release -V
    )
}

#######################################

cmake_build_test ./training/01_Task_Creation/1_xTaskCreateStatic
cmake_build_test ./training/01_Task_Creation/2_xTaskCreate
cmake_build_test ./training/02_Task_Control/2_vTaskDelayUntil
