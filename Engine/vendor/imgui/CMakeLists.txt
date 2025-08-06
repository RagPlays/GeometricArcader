project(ImGui LANGUAGES CXX)

file(GLOB IMGUI_SOURCES *.cpp *.h)
add_library(ImGui STATIC ${IMGUI_SOURCES})

target_compile_features(ImGui PUBLIC cxx_std_17)

if(UNIX)
    set_target_properties(ImGui PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
