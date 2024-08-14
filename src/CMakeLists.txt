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
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    External_lib
    ${SDL2_LIBRARIES}
)

# -------------------------------------------------------------------