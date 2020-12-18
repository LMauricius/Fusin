cmake_minimum_required(VERSION 3.0.0)

# Files
file(
    GLOB_RECURSE
    SOURCES
    "./include/*.h"
    "./src/*.cpp"
)

# Target 
add_library(Fusin ${SOURCES})
set_target_properties(Fusin PROPERTIES OUTPUT_NAME "Fusin${OUTPUT_FILE_SUFFIX}")
target_include_directories(Fusin PUBLIC "./include")

target_compile_definitions(Fusin PRIVATE FUSIN_BUILD)