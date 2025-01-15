@echo off
setlocal

set "BUILD_DIR=build"
set "BUILD_TYPE=Release"
set "EXECUTABLE=Absolomb.exe"

cd "%BUILD_DIR%\bin\%BUILD_TYPE%"
echo Running the executable...
"%EXECUTABLE%"
cd ..\..\..

endlocal
