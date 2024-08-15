# -------------------------------------------------------------------

cmake_minimum_required(VERSION 3.4...3.25 FATAL_ERROR)

# -------------------------------------------------------------------

set(PROJECT_NAME
    Fluid
    LANGUAGES CXX
    VERSION 1.0.0
)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RESOURCES_DIRECTORY 
    "${CMAKE_SOURCE_DIR}/assets"
)

file(COPY ${RESOURCES_DIRECTORY} 
    DESTINATION ${CMAKE_BINARY_DIR}
)

add_compile_options(
    -Wall -Wextra -Wpedantic -Werror -pedantic -g -O3 -ffast-math 
)

add_subdirectory(external)
add_subdirectory(src)

# -------------------------------------------------------------------