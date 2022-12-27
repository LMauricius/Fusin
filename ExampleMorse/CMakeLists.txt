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
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(ExampleMorse PRIVATE Fusin_d)
else()
    target_link_libraries(ExampleMorse PRIVATE Fusin)
endif()