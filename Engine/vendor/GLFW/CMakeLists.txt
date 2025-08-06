cmake_minimum_required(VERSION 3.16)
project(GLFW LANGUAGES C)

# gather all platform-agnostic sources
file(GLOB GLFW_PUBLIC_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/GLFW/*.h"
)
file(GLOB GLFW_BASE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/context.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/init.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/input.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/monitor.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/null_*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/platform.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/vulkan.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/window.c"
)

# now platform-specific
set(GLFW_PLATFORM_SOURCES "")
if(WIN32)
    file(GLOB WIN_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/win32_*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/wgl_context.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/egl_context.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/osmesa_context.c"
    )
    list(APPEND GLFW_PLATFORM_SOURCES ${WIN_SOURCES})
    add_compile_definitions(_GLFW_WIN32 _CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
    file(GLOB COCOA_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cocoa_*.m"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/nsgl_context.m"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/egl_context.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/osmesa_context.c"
    )
    list(APPEND GLFW_PLATFORM_SOURCES ${COCOA_SOURCES})
    add_compile_definitions(_GLFW_COCOA)
elseif(UNIX)
    file(GLOB X11_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/x11_*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/posix_*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/glx_context.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/egl_context.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/osmesa_context.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/linux_joystick.c"
    )
    list(APPEND GLFW_PLATFORM_SOURCES ${X11_SOURCES})
    add_compile_definitions(_GLFW_X11)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

add_library(GLFW STATIC
    ${GLFW_PUBLIC_HEADERS}
    ${GLFW_BASE_SOURCES}
    ${GLFW_PLATFORM_SOURCES}
)

target_include_directories(GLFW PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
