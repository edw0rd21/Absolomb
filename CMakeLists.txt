cmake_minimum_required(VERSION 3.10)

project(Absolomb)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/)

include_directories(include)

add_subdirectory(vendor/SFML)

file(GLOB SOURCE_FILES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

set(SFML_DLLS
    vendor/SFML/bin/sfml-graphics.dll
    vendor/SFML/bin/sfml-window.dll
    vendor/SFML/bin/sfml-system.dll
)

set(ICON_PATH "${CMAKE_SOURCE_DIR}/resources/icon.png")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SFML_DLLS}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${ICON_PATH}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)