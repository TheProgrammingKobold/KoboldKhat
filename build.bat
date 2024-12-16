@echo off
setlocal enabledelayedexpansion

set BUILD_DIR=build

if not exist "%BUILD_DIR%" (
	mkdir "%BUILD_DIR%"
)

cd %BUILD_DIR%

cmake -G "MinGW Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..

cmake --build . --target networking

cmake --build . --target server
cmake --build . --target client

cd ..

