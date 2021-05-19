cmake_minimum_required(VERSION 3.0.0)

# Files
file(
	GLOB_RECURSE
	SOURCES
	"./include/*.h"
	"./src/*.cpp"
)
file(
	GLOB
	HEADERS
	"./include/*"
)

######################################
# Setup target
######################################
add_library(Fusin ${FUSIN_LIB_TYPE} ${SOURCES})
set_target_properties(Fusin PROPERTIES OUTPUT_NAME "Fusin${OUTPUT_FILE_SUFFIX}")
target_include_directories(Fusin PUBLIC "./include")
message("${HEADERS}")
#set_target_properties(Fusin PROPERTIES PUBLIC_HEADER "${HEADERS}")

# link libraries
if (WIN32)
	target_link_libraries(Fusin PUBLIC "hid")
	target_link_libraries(Fusin PUBLIC "XInput")
elseif (UNIX)
	target_link_libraries(Fusin PUBLIC "X11")
	target_link_libraries(Fusin PUBLIC "Xi")
	target_link_libraries(Fusin PUBLIC "Xrandr")
	target_link_libraries(Fusin PUBLIC "xkbcommon")
endif()

# build config
if (FUSIN_STATIC_LIB)
	target_compile_definitions(Fusin PUBLIC FUSIN_STATIC_LIB)
endif()
target_compile_definitions(Fusin PRIVATE FUSIN_BUILD)

# Optional builds
if (FUSIN_NO_DEFAULT_SUBSYSTEMS)
	target_compile_definitions(Fusin PUBLIC FUSIN_NO_DEFAULT_SUBSYSTEMS)
endif()
if (FUSIN_BUILD_RAW_INPUT)
	target_compile_definitions(Fusin PUBLIC FUSIN_BUILD_RAW_INPUT)
endif()
if (FUSIN_BUILD_XINPUT)
	target_compile_definitions(Fusin PUBLIC FUSIN_BUILD_XINPUT)
endif()
if (FUSIN_BUILD_X11)
	target_compile_definitions(Fusin PUBLIC FUSIN_BUILD_X11)
endif()