cmake_minimum_required(VERSION 3.0.0)

# Files
file(
    GLOB_RECURSE
    SOURCES
    "./src/*.h"
    "./src/*.cpp"
)


######################################
# Setup target
######################################
add_executable(ExampleMorse ${SOURCES})
set_target_properties(ExampleMorse PROPERTIES OUTPUT_NAME "ExampleMorse${OUTPUT_FILE_SUFFIX}")
target_include_directories(ExampleMorse PRIVATE "./src")

target_include_directories(ExampleMorse PRIVATE "../Fusin/include")
target_link_libraries(ExampleMorse PRIVATE Fusin)

target_compile_definitions(ExampleMorse PRIVATE FUSIN_BUILD)