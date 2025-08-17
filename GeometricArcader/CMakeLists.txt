cmake_minimum_required(VERSION 3.4)
project("GeometricArcader" LANGUAGES CXX)

# Add FlyFish as a sub-project
add_subdirectory(vendor/FlyFish)

# Recursively grab all .cpp under src/
file(GLOB_RECURSE GEOA_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
if(NOT GEOA_SOURCES)
    message(FATAL_ERROR "No sources found for GeometricArcader in ${CMAKE_CURRENT_SOURCE_DIR}/src")
endif()

add_executable("GeometricArcader" ${GEOA_SOURCES})
set_target_properties(GeometricArcader PROPERTIES OUTPUT_NAME "GeometricArcader")

if (WIN32)
    target_link_options(GeometricArcader PRIVATE
        $<$<CONFIG:Dist>:/SUBSYSTEM:WINDOWS>
        $<$<CONFIG:Dist>:/ENTRY:mainCRTStartup>
    )
endif()

target_include_directories(GeometricArcader PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/Engine/vendor/spdlog/include
    ${CMAKE_SOURCE_DIR}/Engine/src
    ${CMAKE_SOURCE_DIR}/Engine/vendor
    ${CMAKE_SOURCE_DIR}/Engine/vendor/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/FlyFish/include # School FlyFish Lib
)

target_link_libraries(GeometricArcader
    PRIVATE
        Engine
        FlyFish
)

target_compile_definitions(GeometricArcader PRIVATE
    $<$<CONFIG:Debug>:ENGINE_DEBUG>
    $<$<CONFIG:Release>:ENGINE_RELEASE>
    $<$<CONFIG:Dist>:ENGINE_DIST>
    ASSET_PATH="assets/game/"
)

function(copy_assets TARGET_NAME SOURCE_DIR DEST_SUBDIR)
    foreach(config DEBUG RELEASE DIST)
        string(TOUPPER "${config}" _cfg)
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${SOURCE_DIR}"
                "${OUTPUT_BASE}/${config}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/${DEST_SUBDIR}"
            COMMENT "Copying assets from ${SOURCE_DIR} to output dir (${DEST_SUBDIR}) for ${config}"
        )
    endforeach()
endfunction()

# Apply it to both asset folders
copy_assets("GeometricArcader" "${CMAKE_SOURCE_DIR}/Engine/assets" "assets/engine")
copy_assets("GeometricArcader" "${CMAKE_CURRENT_SOURCE_DIR}/assets" "assets/game")