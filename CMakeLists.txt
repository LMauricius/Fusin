cmake_minimum_required(VERSION 3.0)

project(Fusin)
 
# Compiler stuff
get_filename_component(COMPILER_NAME ${CMAKE_CXX_COMPILER} NAME_WE)

string(REPLACE "." ";" COMPILER_VERSION ${CMAKE_CXX_COMPILER_VERSION})
list(GET COMPILER_VERSION 0 COMPILER_VERSION_MAJOR)
list(GET COMPILER_VERSION 1 COMPILER_VERSION_MINOR)
set(COMPILER_VERSION "${COMPILER_VERSION_MAJOR}.${COMPILER_VERSION_MINOR}")

# Decide on locations
#if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")#if (CMAKE_CXX_COMPILER_ID EQUAL "MSVC")# Visual studio
#    set(OUTPUT_DIR_SUFFIX "/${COMPILER_NAME}_${CMAKE_VS_PLATFORM_TOOLSET}_${CMAKE_VS_PLATFORM_NAME}")
#else()# Other compilers
#    set(OUTPUT_DIR_SUFFIX "/${CMAKE_CXX_COMPILER_ID}_${CMAKE_CXX_COMPILER_VERSION}")
#endif()
set(OUTPUT_DIR_SUFFIX "")

######################################
# User build configuration
######################################

# Build options
if (WIN32)
  set(FUSIN_STATIC_LIB false CACHE BOOL "Builds Fusin as a static library instead of a DLL")
endif()

# Choose which subsystems to build
set(FUSIN_NO_DEFAULT_SUBSYSTEMS false CACHE BOOL "Prevents from building default subsystems for a platform")
if (FUSIN_NO_DEFAULT_SUBSYSTEMS)
  if (WIN32)
    set(FUSIN_BUILD_RAW_INPUT false CACHE BOOL "Builds RawInput subsystem")
    set(FUSIN_BUILD_XINPUT false CACHE BOOL "Builds XInput subsystem")
  elseif (UNIX)
    set(FUSIN_BUILD_X11 false CACHE BOOL "Builds X11 IO subsystem")
  endif()
endif()

# Build non-essentials
set(FUSIN_BUILD_EXAMPLES true CACHE BOOL "Build example executables")

######################################
# Global settings
######################################

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin${OUTPUT_DIR_SUFFIX}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin${OUTPUT_DIR_SUFFIX}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin${OUTPUT_DIR_SUFFIX}")

# Determine the library type
if (FUSIN_STATIC_LIB)
  set(FUSIN_LIB_TYPE STATIC)
else()
  set(FUSIN_LIB_TYPE SHARED)
endif()

# Decide on filenames
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(OUTPUT_FILE_SUFFIX "_d")
else()
  set(OUTPUT_FILE_SUFFIX "")
endif()

######################################
# Add subdirectories
######################################
add_subdirectory("./Fusin")
if (FUSIN_BUILD_EXAMPLES)
  add_subdirectory("./ExampleBasics")
endif()

######################################
# Install stuff
######################################
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)