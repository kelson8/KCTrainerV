# TODO Fix this up
# It gives errors with Logger.hpp and quite a few more
# And I need to fix this to sort in Visual Studio like how I currently have setup.

cmake_minimum_required (VERSION 3.22)

set(PROJECT_NAME "KCTrainerV")
set(APP_VERSION "0.5.0")
#set(APP_VERSION_SUFFIX "a")  # Pre-release suffix

set(SCRIPTHOOKV_LIB "ScriptHookV.lib")

#--------------
# Source files
#--------------

# Get most source files
file(GLOB MAIN_SOURCES 
	"KCTrainerV-menu/*.cpp" 
	"KCTrainerV-menu/Components/*.cpp" 
	"KCTrainerV-menu/Memory/*.cpp" 
	"KCTrainerV-menu/Memory/Hooks*.cpp" 
	
	"KCTrainerV-menu/Menus*.cpp"
	
    "KCTrainerV-menu/Menus/Submenus/Misc/*.cpp"
    "KCTrainerV-menu/Menus/Submenus/Ped/*.cpp"
    "KCTrainerV-menu/Menus/Submenus/Player/*.cpp"
    "KCTrainerV-menu/Menus/Submenus/Teleport/*.cpp"
    "KCTrainerV-menu/Menus/Submenus/Vehicle/*.cpp"
    "KCTrainerV-menu/Menus/Submenus/Weapon/*.cpp"
    "KCTrainerV-menu/Menus/Submenus/World/*.cpp"
	
	
	"KCTrainerV-menu/Natives/*.cpp"
	
	"KCTrainerV-menu/Scripts/*.cpp" 
	"KCTrainerV-menu/Scripts/Extras/*.cpp" 
	"KCTrainerV-menu/Scripts/Extras/Classes*.cpp"
	"KCTrainerV-menu/Scripts/Markers/*.cpp"
	"KCTrainerV-menu/Scripts/Misc*.cpp"
	"KCTrainerV-menu/Scripts/Peds/*.cpp" 
	"KCTrainerV-menu/Scripts/Player/*.cpp" 
	"KCTrainerV-menu/Scripts/Vehicles/*.cpp" 
	
	"KCTrainerV-menu/Teleports/*.cpp" 
	"KCTrainerV-menu/Util/*.cpp"
	)
	
# Get most sources from thirdparty folder
file(GLOB THIRDPARTY_SOURCES

	"thirdparty/GTAVMenuBase/*.cpp"
	
	"thirdparty/minhook/src/*.c"
	"thirdparty/minhook/src/hde/*.c"
	"thirdparty/Patterns/*.cpp"
	
	"thirdparty/shv/*.cpp"
	)


#--------------
# Flags
#--------------

# Toggle optimizations for the project in release
# -O0	No optimization, useful for debugging.
# -O1	Basic optimizations without a significant increase in compile time.
# -O2	Moderate optimizations, suitable for most projects. Often a good compromise between performance and compile time.
# -O3	Aggressive optimizations, may improve performance but can increase compile time and binary size.
# -Os	Optimize for code size, enabling optimizations that reduce size.
# -Ofast	Disregards strict standards compliance for more aggressive optimizations.
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

#--------------
# Project name, C++ language, and version
#--------------

project(${PROJECT_NAME}
  LANGUAGES CXX
  VERSION ${APP_VERSION}
)

# Specify the path to the version.h.in file
set(VERSION_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in")
set(GENERATED_VERSION_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h")

# Generate a header file with version info
# configure_file(
    # "${VERSION_HEADER}"
    # "${GENERATED_VERSION_HEADER}"
    # @ONLY
# )

# Adapted from the CMake file here
# https://github.com/ArthurSonzogni/ftxui-starter/blob/master/CMakeLists.txt

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#--------------
# KCTrainerV libraries
#--------------

if(WIN32)
	link_directories(thirdparty/ScriptHookV_SDK/lib)

	# GTAV-Classes, this has a CMake so that should help.
	add_subdirectory(thirdparty/GTAV-Classes)
	
	# Minhook
	add_subdirectory(thirdparty/minhook)
	
	# Lua sol
	add_subdirectory(thirdparty/sol)
	
	# TinyXml
	add_subdirectory(thirdparty/tinyxml2)

endif()

# --- Fetch FMT Library --------------------------------------------------------------
# Possibly enable this later?
# include(FetchContent)

# set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
# set(FETCHCONTENT_QUIET OFF)

# FetchContent_Declare(fmt
  # GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  # GIT_TAG        12.1.0
  # GIT_PROGRESS   TRUE
  # GIT_SHALLOW    TRUE
  # EXCLUDE_FROM_ALL
# )
# FetchContent_MakeAvailable(fmt)
# ------------------------------------------------------------------------------

#-------------
# Spdlog
# Adapted from here
# https://github.com/twanas/spdlog-cmake/blob/master/CMakeLists.txt
# TODO Switch to using this in KCTrainerV
#-------------

# Using add_subdirectory doesn't require find_package
# So if I have git submodules I can just include them like this.
# add_subdirectory("include/spdlog")

#-------------
# Setup main project
#-------------

add_executable(${PROJECT_NAME}
    ${MAIN_SOURCES}
	${THIRDPARTY_SOURCES}
    )

#-------------
# Add compiler flags
#-------------

# Disable all these warnings for now
# if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # target_compile_options(${PROJECT_NAME} PRIVATE -Wunused-variable -Wall -Wextra)
# elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # target_compile_options(${PROJECT_NAME} PRIVATE /W4)
# endif()


#-------------
# Include directories
#-------------
if(WIN32)
target_include_directories(${PROJECT_NAME} PRIVATE
	  KCTrainerV-Menu/
      KCTrainerV-menu/Components
      KCTrainerV-menu/Lib
      
	  KCTrainerV-menu/Memory
      KCTrainerV-menu/Memory/Hooks
	  
      KCTrainerV-menu/Menus
      KCTrainerV-menu/Menus/Submenus
      KCTrainerV-menu/Menus/Submenus/Misc
      KCTrainerV-menu/Menus/Submenus/Ped
      KCTrainerV-menu/Menus/Submenus/Player
      KCTrainerV-menu/Menus/Submenus/Teleport
      KCTrainerV-menu/Menus/Submenus/Vehicle
      KCTrainerV-menu/Menus/Submenus/Weapon
      KCTrainerV-menu/Menus/Submenus/World
      
	  KCTrainerV-menu/Natives
	  
      KCTrainerV-menu/Scripts
      KCTrainerV-menu/Scripts/Extras
      KCTrainerV-menu/Scripts/Markers
      KCTrainerV-menu/Scripts/Misc
      KCTrainerV-menu/Scripts/Peds
      KCTrainerV-menu/Scripts/Player
      KCTrainerV-menu/Scripts/Scaleform
      KCTrainerV-menu/Scripts/Stats
      KCTrainerV-menu/Scripts/Vehicles
      KCTrainerV-menu/Scripts/World
	  
      KCTrainerV-menu/Teleports
      KCTrainerV-menu/Util
	  
	  thirdparty/
	  thirdparty/GTAVMenuBase
	  thirdparty/json/include
	  
	  # thirdparty/minhook/include
	  thirdparty/Patterns
	  thirdparty/ScriptHookV_SDK
	  thirdparty/shv
	  thirdparty/simpleini
	  thirdparty/sol/include
	  thirdparty/tinyxml2
	  )
endif()

#-------------
# Link libraries
# TODO Hookup ScriptHookV to this
#-------------
if(WIN32)
  target_link_libraries(${PROJECT_NAME}
  PRIVATE ${SCRIPTHOOKV_LIB}
  )

endif()
