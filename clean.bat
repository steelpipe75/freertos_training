@echo off

call :clean_build .\training\01_Task_Creation\1_xTaskCreateStatic
call :clean_build .\training\01_Task_Creation\2_xTaskCreate
call :clean_build .\training\02_Task_Control\1_vTaskDelay
call :clean_build .\training\02_Task_Control\2_vTaskDelayUntil
call :clean_build .\training\02_Task_Control\3_xTaskDelayUntil
call :clean_build .\training\02_Task_Control\4_vTaskPrioritySet

call :clean_build .\Demo_CMake\FreeRTOS_Plus_CLI_with_Trace_Windows_Simulator

exit /b

:clean_build
echo ==============================================================================
echo %1
echo ------------------------------------------------------------------------------
rmdir /S /Q %1\build
exit /b
