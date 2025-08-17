project(Engine LANGUAGES CXX)

add_library(Engine STATIC)

# Precompiled header (Visual Studio only)
if(MSVC)
    target_precompile_headers(Engine PRIVATE src/enginepch.h)
endif()

# Source files
file(GLOB_RECURSE ENGINE_SOURCES
    src/*.cpp
    src/*.h
    vendor/stb_image/*.cpp
    vendor/stb_image/*.h
)
target_sources(Engine PRIVATE ${ENGINE_SOURCES})

# Public include directories
target_include_directories(Engine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Engine/vendor/spdlog/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Engine/vendor/GLFW/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Engine/vendor/Glad/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Engine/vendor/imgui>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Engine/vendor/glm>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Engine/vendor/stb_image>
)

# Compiler definitions
target_compile_definitions(Engine
    PUBLIC
        _CRT_SECURE_NO_WARNINGS
        GLFW_INCLUDE_NONE
)

# Force GLFW to build static
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Add third-party dependencies first
add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/Glad)
add_subdirectory(vendor/imgui)

# Link dependencies
target_link_libraries(Engine
    PUBLIC
        glfw
        Glad
        ImGui
        opengl32        # Windows OpenGL
)

# Per-configuration definitions/flags
target_compile_definitions(Engine PRIVATE
    $<$<CONFIG:Debug>:ENGINE_DEBUG>
    $<$<CONFIG:Release>:ENGINE_RELEASE>
    $<$<CONFIG:Dist>:ENGINE_DIST>
    ASSET_PATH="assets/engine/"
)
