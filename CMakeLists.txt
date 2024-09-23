cmake_minimum_required(VERSION 3.10)
project(Absolomb)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(SFML 2.6.1 COMPONENTS graphics window system REQUIRED)
if(SFML_FOUND)
    target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
endif()