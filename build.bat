@echo off
setlocal enabledelayedexpansion

set BUILD_DIR=build
set COMPILE_COMMANDS=compile_commands.json

if exist "%BUILD_DIR%" (
	echo Deleting preexisting build directory...
	del "%BUILD_DIR%"
) else (
	mkdir "%BUILD_DIR%"
)

cd %BUILD_DIR%

cmake -G "MinGW Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..

cmake --build . --target networking
cmake --build . --target server
cmake --build . --target client

if exist "%COMPILE_COMMANDS%" (
	copy "%COMPILE_COMMANDS%" ..
	echo Copied %COMPILE_COMMANDS% to the root directory.
) else (
	echo Warning: %COMPILE_COMMANDS% not found in build directory.
)

cd ..

