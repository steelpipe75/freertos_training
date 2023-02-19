@echo off

call :cmake_build_test .\training\01_Task_Creation\1_xTaskCreateStatic
call :cmake_build_test .\training\01_Task_Creation\2_xTaskCreate
call :cmake_build_test .\training\02_Task_Control\1_vTaskDelay
call :cmake_build_test .\training\02_Task_Control\2_vTaskDelayUntil
call :cmake_build_test .\training\02_Task_Control\3_xTaskDelayUntil

exit /b

:cmake_build_test
echo ==============================================================================
echo %1
echo ------------------------------------------------------------------------------
cmake -B %1\build -S %1 -DCMAKE_BUILD_TYPE=Release -G Ninja
echo ------------------------------------------------------------------------------
cmake --build %1\build --config Release
echo ------------------------------------------------------------------------------
pushd %1\build
ctest -C Release -V
popd
echo ------------------------------------------------------------------------------
exit /b
