cmake_minimum_required(VERSION 3.5...4.1)

# Three configs: Debug, Release, and Dist
set(CMAKE_CONFIGURATION_TYPES
    "Debug;Release;Dist"
    CACHE STRING "Configs for all generators" FORCE
)

project(GeometricArcaderSolution
  VERSION 1.0
  DESCRIPTION "Workspace"
  LANGUAGES C CXX
)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define output dirs per config
set(OUTPUT_BASE "${CMAKE_SOURCE_DIR}/bin")
set(OUTPUT_OBJ "${CMAKE_SOURCE_DIR}/bin-int")

foreach(config DEBUG RELEASE DIST)
    string(TOUPPER "${config}" _cfg)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${_cfg}
        "${OUTPUT_BASE}/${config}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${_cfg}
        "${OUTPUT_OBJ}/${config}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
endforeach()

if (MSVC)
  # Force static runtime linking
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  
  # Enable UTF-8 source encoding so spdlog’s fmt bundle is happy
  add_compile_options(/utf-8)
  # multithreaded compilation
  add_compile_options(/MP)
endif()

# Engine library
add_subdirectory(Engine)

# Executables
add_subdirectory(GeometricArcader)