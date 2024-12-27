# Require CMAKE 3.12 or higher
cmake_minimum_required(VERSION 3.12)

# Project name
project(itp380labs)

# Build for C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Where any include files are
# Every target needs stb/rapidjson includes
include_directories(Libraries/stb/include)
include_directories(Libraries/rapidjson/include)
# For emscripten, we don't include GLEW/SDL because they're provided by emsdk
if (NOT EMSCRIPTEN)
	include_directories(Libraries/GLEW/include)
	include_directories(Libraries/SDL/include)
endif()

# Where any libraries are
if (WIN32)
	link_directories(Libraries/GLEW/lib/win/x64)
	link_directories(Libraries/SDL/lib/win/x64)
elseif(APPLE)
	link_directories(Libraries/GLEW/lib/mac)
	link_directories(Libraries/SDL/lib/mac)
endif()

# Set any compiler-specific settings
if (WIN32)
	set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /W3 /GR /EHsc /wd26812 /w34456 /w34457 /w34458 /w34459 /w35038 /we4715 /we4834 /we4172")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4099 /ignore:4075")
elseif(APPLE)
	set(CMAKE_CXX_FLAGS "-Wall -Werror=return-type -Werror=unused-result -Werror=return-stack-address -ftrivial-auto-var-init=pattern")
	find_library(COREFOUNDATION_LIBRARY CoreFoundation)
	find_library(COREGRAPHICS_LIBRARY CoreGraphics)
	find_library(COREAUDIO_LIBRARY CoreAudio)
	find_library(COREVIDEO_LIBRARY CoreVideo)
	find_library(AUDIOTOOLBOX_LIBRARY AudioToolbox)
	find_library(FORCEFEEDBACK_LIBRARY ForceFeedback)
	find_library(IOKIT_LIBRARY IOKit)
	find_library(APPKIT_LIBRARY AppKit)
	find_library(METAL_LIBRARY Metal)
	find_library(CARBON_LIBRARY Carbon)
	find_library(OPENGL_LIBRARY OpenGL)
	set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Debug] "YES")
	set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=MinSizeRel] "NO")
	set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=RelWithDebInfo] "YES")
	set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Release] "NO")
elseif (EMSCRIPTEN)
	set(CMAKE_CXX_FLAGS "-sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS=\"[\"png\"]\" -sUSE_SDL_MIXER=2 -sUSE_SDL_TTF=2")
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -sMIN_WEBGL_VERSION=2 -sTOTAL_MEMORY=1024MB --shell-file \"${PROJECT_SOURCE_DIR}/Libraries/Emscripten/shell_minimal.html\"")
endif()

# Subdirectories to build
add_subdirectory(Lab01)
add_subdirectory(Lab02)
add_subdirectory(Lab03)
add_subdirectory(Lab04)
add_subdirectory(Lab05)
add_subdirectory(Lab06)
add_subdirectory(Lab07)
add_subdirectory(Lab08)
add_subdirectory(Lab09)
add_subdirectory(Lab10)
add_subdirectory(Lab11)
add_subdirectory(Lab12)
