#!/bin/bash

#######################################

clean_build() {
    echo ==============================================================================
    echo $1
    echo ------------------------------------------------------------------------------
    rm -r $1/build
}

#######################################

clean_build ./training/01_Task_Creation/1_xTaskCreateStatic
clean_build ./training/01_Task_Creation/2_xTaskCreate
clean_build ./training/02_Task_Control/1_vTaskDelay
clean_build ./training/02_Task_Control/2_vTaskDelayUntil
clean_build ./training/02_Task_Control/3_xTaskDelayUntil
