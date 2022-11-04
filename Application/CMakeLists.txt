cmake_minimum_required (VERSION 3.8)

project ("Application" VERSION 1.0.0)

set(CMAKE_SUPPRESS_REGENERATION false)
set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB _root_files		LIST_DIRECTORIES false "${_src_root_path}/*.hpp"		"${_src_root_path}/*.h"				"${_src_root_path}/*.cpp")
file(GLOB _handle_files		LIST_DIRECTORIES false "${_src_root_path}/Handle/*.h"	"${_src_root_path}/Handle/*.hpp"	"${_src_root_path}/Handle/*.cpp")
file(GLOB _platform_files	LIST_DIRECTORIES false "${_src_root_path}/Platform/*.h" "${_src_root_path}/Platform/*.hpp"	"${_src_root_path}/Platform/*.cpp")
file(GLOB _worker_files		LIST_DIRECTORIES false "${_src_root_path}/Worker/*.h"	"${_src_root_path}/Worker/*.hpp"	"${_src_root_path}/Worker/*.cpp")

add_executable (Application 
	${_root_files}
	${_handle_files}
	${_platform_files}
	${_worker_files}
)

source_group("/"			FILES ${_root_files})
source_group("/Handle"		FILES ${_handle_files})
source_group("/Platform"	FILES ${_platform_files})
source_group("/Worker"		FILES ${_worker_files})

# Resolve __cplusplus flag to be set accurate
target_compile_options(Application PUBLIC "/Zc:__cplusplus")

if(MSVC)
  #target_compile_options(Application PRIVATE /W4 /WX)
else()
  target_compile_options(Application PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

include_directories(
	"/"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Application PROPERTY CXX_STANDARD 20)
endif()
