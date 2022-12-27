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
add_executable(ExampleBasics ${SOURCES})
set_target_properties(ExampleBasics PROPERTIES OUTPUT_NAME "ExampleBasics${OUTPUT_FILE_SUFFIX}")
target_include_directories(ExampleBasics PRIVATE "./src")

target_include_directories(ExampleBasics PRIVATE "../Fusin/include")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(ExampleBasics PUBLIC Fusin_d)
else()
    target_link_libraries(ExampleBasics PUBLIC Fusin)
endif()