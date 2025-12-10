cmake_minimum_required(VERSION 3.20)
project(RocketSim C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic -Wshadow")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SDL2 REQUIRED)

add_executable(controller
    src/controller/main.c
)
add_executable(simulator
    src/simulator/main.c
)

add_executable(visualizer
    src/visualizer/main.c
)
target_include_directories(visualizer PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(visualizer PRIVATE ${SDL2_LIBRARIES})

# RocketSim
add_executable(rocketsim
    src/launcher/main.c
)
target_include_directories(rocketsim PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(rocketsim PRIVATE ${SDL2_LIBRARIES})
target_compile_options(rocketsim PRIVATE $<$<CONFIG:Debug>:-fsanitize=address -fno-omit-frame-pointer>)
target_link_options(rocketsim PRIVATE $<$<CONFIG:Debug>:-fsanitize=address>)


