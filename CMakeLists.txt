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

set(JSON_FILE 
    ${CMAKE_SOURCE_DIR}/simulation_params.json
)

file(COPY ${JSON_FILE}
    DESTINATION ${CMAKE_BINARY_DIR}
)

add_compile_options(
    -Wall -Wextra -Wpedantic -Werror -pedantic -g -Ofast 
)

add_subdirectory(external)
add_subdirectory(src)

# -------------------------------------------------------------------