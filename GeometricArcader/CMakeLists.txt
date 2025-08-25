cmake_minimum_required(VERSION 3.5...4.1)
project("GeometricArcader" LANGUAGES CXX)

# Add FlyFish as a sub-project
add_subdirectory(vendor/FlyFish)

# Recursively grab all files
file(GLOB_RECURSE GEOA_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)
if(NOT GEOA_SOURCES)
    message(FATAL_ERROR "No sources found for GeometricArcader in ${CMAKE_CURRENT_SOURCE_DIR}/src")
endif()

# Create the executable
add_executable(GeometricArcader ${GEOA_SOURCES})

# Mirror actual folder hierarchy in Visual Studio
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PREFIX ""
    FILES ${GEOA_SOURCES}
)

# Set executable name
set_target_properties(GeometricArcader PROPERTIES
    OUTPUT_NAME "GeometricArcader"
)

# Visual Studio specific settings
if(MSVC)
    # Run exe from its output directory
    set_target_properties(GeometricArcader PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:GeometricArcader>"
        VS_DEBUGGER_COMMAND "$<TARGET_FILE:GeometricArcader>"
    )
endif()

# Windows-specific linker options
if(WIN32)
    target_link_options(GeometricArcader PRIVATE
        $<$<CONFIG:Dist>:/SUBSYSTEM:WINDOWS>
        $<$<CONFIG:Dist>:/ENTRY:mainCRTStartup>
    )
endif()

# Include directories
target_include_directories(GeometricArcader PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/Engine/vendor/spdlog/include
    ${CMAKE_SOURCE_DIR}/Engine/src
    ${CMAKE_SOURCE_DIR}/Engine/vendor
    ${CMAKE_SOURCE_DIR}/Engine/vendor/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/FlyFish/include
)

# Link libraries
target_link_libraries(GeometricArcader PRIVATE
    Engine
    FlyFish
)

# Compile definitions
target_compile_definitions(GeometricArcader PRIVATE
    $<$<CONFIG:Debug>:ENGINE_DEBUG>
    $<$<CONFIG:Release>:ENGINE_RELEASE>
    $<$<CONFIG:Dist>:ENGINE_DIST>
    ASSET_PATH="assets/game/"
)

# Function to copy assets after build
function(copy_assets TARGET_NAME SOURCE_DIR DEST_SUBDIR)
    string(TOUPPER "$<CONFIG>" _cfg)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${SOURCE_DIR}"
            "${OUTPUT_BASE}/$<CONFIG>-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/${DEST_SUBDIR}"
        COMMENT "Copying assets from ${SOURCE_DIR} to output dir (${DEST_SUBDIR}) for $<CONFIG>"
    )
endfunction()

# Apply asset copying
copy_assets("GeometricArcader" "${CMAKE_SOURCE_DIR}/Engine/assets" "assets/engine")
copy_assets("GeometricArcader" "${CMAKE_CURRENT_SOURCE_DIR}/assets" "assets/game")
