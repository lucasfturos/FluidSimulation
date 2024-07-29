# -------------------------------------------------------------------

set(SRC_HEADER
    Render/render.hpp
    FluidSimulation/fluid_simulation.hpp
)

set(SRC_SOURCE
    FluidSimulation/draw.cpp
    FluidSimulation/fluid_simulation.cpp
    FluidSimulation/logic.cpp
    FluidSimulation/util.cpp
    Render/render.cpp
    Render/setup.cpp
    main.cpp 
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} 
    ${SRC_HEADER} 
    ${SRC_SOURCE}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${JSONCPP_LIBRARIES}
    PRIVATE ${SDL2_LIBRARIES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${JSONCPP_LIBRARIES}
    PRIVATE ${SDL2_LIBRARIES}
)

# -------------------------------------------------------------------