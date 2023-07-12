@echo off

copy /Y "%~dp0port.c" "%~dp0Middlewares\Third_Party\FreeRTOS\Source\portable\RVDS\ARM_CM4F\"

copy /Y "%~dp0portmacro.h" "%~dp0Middlewares\Third_Party\FreeRTOS\Source\portable\RVDS\ARM_CM4F\"