@echo off

call :clean_build .\training\01_Task_Creation\1_xTaskCreateStatic
call :clean_build .\training\01_Task_Creation\2_xTaskCreate
call :clean_build .\training\02_Task_Control\1_vTaskDelay
call :clean_build .\training\02_Task_Control\2_vTaskDelayUntil
call :clean_build .\training\02_Task_Control\3_xTaskDelayUntil

exit /b

:clean_build
echo ==============================================================================
echo %1
echo ------------------------------------------------------------------------------
rmdir /S /Q %1\build
exit /b
