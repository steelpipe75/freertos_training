#!/bin/bash

#######################################

cmake -B ./training/01_Task_Creation/1_xTaskCreateStatic/build -S ./training/01_Task_Creation/1_xTaskCreateStatic -DCMAKE_BUILD_TYPE=Release
cmake --build ./training/01_Task_Creation/1_xTaskCreateStatic/build --config Release

(
    cd ./training/01_Task_Creation/1_xTaskCreateStatic/build
    ctest -C Release -V
)

#######################################

cmake -B ./training/01_Task_Creation/2_xTaskCreate/build -S ./training/01_Task_Creation/2_xTaskCreate -DCMAKE_BUILD_TYPE=Release
cmake --build ./training/01_Task_Creation/2_xTaskCreate/build --config Release

(
    cd ./training/01_Task_Creation/2_xTaskCreate/build
    ctest -C Release -V
)

#######################################

cmake -B ./training/02_Task_Control/2_vTaskDelayUntil/build -S ./training/02_Task_Control/2_vTaskDelayUntil -DCMAKE_BUILD_TYPE=Release
cmake --build ./training/02_Task_Control/2_vTaskDelayUntil/build --config Release

(
    cd ./training/02_Task_Control/2_vTaskDelayUntil/build
    ctest -C Release -V
)
