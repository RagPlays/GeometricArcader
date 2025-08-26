add_library(Engine STATIC)

# Precompiled header
target_precompile_headers(Engine PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/enginepch.h>
)

# Source files
file(GLOB_RECURSE ENGINE_SOURCES
    src/*.cpp
    src/*.h
    vendor/stb_image/*.cpp
    vendor/stb_image/*.h
)
target_sources(Engine
    PRIVATE
        ${ENGINE_SOURCES}
)

# Public include directories
target_include_directories(Engine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/Glad/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image>
)

# Compiler definitions
target_compile_definitions(Engine
    PRIVATE
        _CRT_SECURE_NO_WARNINGS #MSVC specific
        GLFW_INCLUDE_NONE
        ASSET_PATH="assets/engine/"
)

target_compile_definitions(Engine
    PUBLIC
        $<$<CONFIG:Debug>:ENGINE_DEBUG>
        $<$<CONFIG:Release>:ENGINE_RELEASE>
        $<$<CONFIG:Dist>:ENGINE_DIST>
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