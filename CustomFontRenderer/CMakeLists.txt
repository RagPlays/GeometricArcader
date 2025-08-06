cmake_minimum_required(VERSION 3.16)
project(CustomFontRenderer LANGUAGES CXX)

# Recursively grab all .cpp under src/
file(GLOB_RECURSE CFR_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

if(NOT CFR_SOURCES)
    message(FATAL_ERROR "No sources found for CustomFontRenderer in ${CMAKE_CURRENT_SOURCE_DIR}/src")
endif()

add_executable(CustomFontRenderer ${CFR_SOURCES})

target_include_directories(CustomFontRenderer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/Engine/vendor/spdlog/include
    ${CMAKE_SOURCE_DIR}/Engine/src
    ${CMAKE_SOURCE_DIR}/Engine/vendor
    ${CMAKE_SOURCE_DIR}/Engine/vendor/glm
)

target_link_libraries(CustomFontRenderer PRIVATE Engine)

target_compile_definitions(CustomFontRenderer PRIVATE
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
copy_assets(CustomFontRenderer "${CMAKE_SOURCE_DIR}/Engine/assets" "assets/engine")
copy_assets(CustomFontRenderer "${CMAKE_CURRENT_SOURCE_DIR}/assets" "assets/game")