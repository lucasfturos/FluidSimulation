# -------------------------------------------------------------------

set(SRC_HEADER
    # Fluid
    Fluid/fluid.hpp
    # Render
    Render/render.hpp
)

set(SRC_SOURCE
    # Fluid
    Fluid/draw.cpp
    Fluid/fluid.cpp
    Fluid/logic.cpp
    Fluid/util.cpp
    # Render
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
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    External_lib
    ${SDL2_LIBRARIES}
)

# -------------------------------------------------------------------