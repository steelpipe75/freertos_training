#!/bin/bash

#######################################

clean_build() {
    echo $1
    rm -r $1/build
}

#######################################

clean_build ./training/01_Task_Creation/1_xTaskCreateStatic
clean_build ./training/01_Task_Creation/2_xTaskCreate
clean_build ./training/02_Task_Control/2_vTaskDelayUntil
