@echo off
setlocal enabledelayedexpansion

set BUILD_DIR=build
set BUILD_TYPE=Release

if not exist "%BUILD_DIR%" (
    echo Creating build directory: %BUILD_DIR%
    mkdir "%BUILD_DIR%"
)

cd "%BUILD_DIR%"

echo Configuring the project...
cmake .. -DCMAKE_BUILD_TYPE=%BUILD_TYPE%

echo Building the project...
cmake --build . --config %BUILD_TYPE%

cd ..

echo Build complete!

