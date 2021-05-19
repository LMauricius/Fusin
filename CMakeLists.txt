cmake_minimum_required(VERSION 3.0)

project(Fusin)

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


# =========================
# Global settings
# =========================

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

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

# =========================
# Add subdirectories
# =========================
add_subdirectory("./Fusin")
set(TARGET_LIST "Fusin")
if (FUSIN_BUILD_EXAMPLES)
  add_subdirectory("./ExampleBasics")
  add_subdirectory("./ExampleMorse")
  #set(TARGET_LIST ${TARGET_LIST} ExampleBasics ExampleMorse) # Don't install examples
endif()

# =========================
# Install stuff
# =========================
message("${TARGET_LIST}")
install(
  TARGETS ${TARGET_LIST}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)
install(
  DIRECTORY ${PROJECT_SOURCE_DIR}/Fusin/include/
  DESTINATION include/Fusin
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)