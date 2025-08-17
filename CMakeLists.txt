cmake_minimum_required(VERSION 3.4)

# Tell VS that our solution has three configs: Debug, Release, and Dist
set(CMAKE_CONFIGURATION_TYPES
    "Debug;Release;Dist"
    CACHE STRING "Configs for all generators" FORCE
)

project(GeometricArcaderSolution
  VERSION 1.0
  DESCRIPTION "Workspace"
  LANGUAGES C CXX
)

# Multi-config generators (Visual Studio): define output dirs per config
set(OUTPUT_BASE "${CMAKE_SOURCE_DIR}/bin")
foreach(config DEBUG RELEASE DIST)
    string(TOUPPER "${config}" _cfg)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${_cfg}
        "${OUTPUT_BASE}/${config}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${_cfg}
        "${OUTPUT_BASE}/${config}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
endforeach()

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# For MSVC users, enable UTF-8 source encoding so spdlog’s fmt bundle is happy
if (MSVC)
  add_compile_options(/utf-8)
  add_compile_options(/MP)
endif()

# Engine library
add_subdirectory(Engine)

# Executables
add_subdirectory(GeometricArcader)
add_subdirectory(Sandbox)
add_subdirectory(CustomFontRenderer)