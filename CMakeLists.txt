cmake_minimum_required(VERSION 4.2)
project(RocketSim C)

# C STD
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF) 

# Compile Commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic -Wshadow")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# GLAD
set(GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad")
set(GLAD_SOURCE "${GLAD_DIR}/src/glad.c")
set(GLAD_INCLUDE "${GLAD_DIR}/include")
# - warning check
if(NOT EXISTS "${GLAD_SOURCE}")
    message(FATAL_ERROR
        "\n---- GLAD Missing! ----\n"
        "Could not find glad.c at: ${GLAD_SOURCE}\n"
        "Please generate it at https://glad.dav1d.de/ and place the "
        "'src' and 'include' folders into 'vendor/glad/'.\n"
        "---------------------------------------------\n"
    )
endif()
add_library(glad STATIC "${GLAD_SOURCE}")
target_include_directories(glad PUBLIC "${GLAD_INCLUDE}")
# -> turn off compile warnings for glad (external)
target_compile_options(glad PRIVATE -w) # does not work for msvc

# GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)
find_package(glfw3 REQUIRED)

# Create 4 executables: controller, simulator, visualizer, and rocketsim
# - Controller
add_executable(controller
    src/controller/main.c
)
target_include_directories(controller
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src
)

# - Simulator
add_executable(simulator
    src/simulator/main.c
)
target_include_directories(simulator
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src
)

# - Visualizer
add_executable(visualizer
    src/visualizer/main.c
    src/visualizer/objects/triangle.c
    src/rsm.c
)
target_link_libraries(visualizer PRIVATE glfw glad)
target_include_directories(visualizer
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src
)

# RocketSim
add_executable(rocketsim
    src/launcher/main.c
)
target_include_directories(rocketsim
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src
)
