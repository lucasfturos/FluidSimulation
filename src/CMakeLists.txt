# -------------------------------------------------------------------

cmake_minimum_required(VERSION 3.4...3.25 FATAL_ERROR)

# -------------------------------------------------------------------

set(SRC_HEADER
    # ControlPanel
    ControlPanel/control_panel.hpp
    # Fluid
    Fluid/fluid.hpp
    # Render
    Render/render.hpp
)

set(SRC_SOURCE
    # ControlPanel
    ControlPanel/control_panel.cpp
    ControlPanel/gui.cpp
    # Fluid
    Fluid/fluid.cpp
    Fluid/logic.cpp
    Fluid/util.cpp
    # Render
    Render/event.cpp
    Render/render.cpp
    Render/setup.cpp
    # Main
    main.cpp 
)

find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME} 
    ${SRC_HEADER} 
    ${SRC_SOURCE}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    External_lib
    ${SDL2_LIBRARIES}
)

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    
    target_compile_options(${PROJECT_NAME} PRIVATE
        -s USE_SDL=2
    )
    
    target_link_options(${PROJECT_NAME} PRIVATE
        "-s WASM=1"
        "-s ALLOW_MEMORY_GROWTH=1"
        "-s NO_EXIT_RUNTIME=0"
        "-s ASSERTIONS=1"
        --no-heap-copy 
        --preload-file ${CMAKE_SOURCE_DIR}/assets/font@/assets/font
        --shell-file ${CMAKE_SOURCE_DIR}/html/template.html
    )
endif()

# -------------------------------------------------------------------