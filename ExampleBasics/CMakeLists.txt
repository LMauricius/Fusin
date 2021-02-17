cmake_minimum_required(VERSION 3.0.0)

# Files
file(
    GLOB_RECURSE
    SOURCES
    "./src/*.h"
    "./src/*.cpp"
)

# Target
add_executable(ExampleBasics ${SOURCES})
set_target_properties(ExampleBasics PROPERTIES OUTPUT_NAME "ExampleBasics${OUTPUT_FILE_SUFFIX}")
target_include_directories(ExampleBasics PRIVATE "./src")
#target_link_directories(ExampleBasics PRIVATE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
target_include_directories(ExampleBasics PRIVATE "../Fusin/include")
target_link_libraries(ExampleBasics PRIVATE Fusin)

target_compile_definitions(ExampleBasics PRIVATE FUSIN_BUILD)