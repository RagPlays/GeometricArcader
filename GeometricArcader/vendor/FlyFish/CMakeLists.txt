project(FlyFish LANGUAGES CXX)

# 1) Build the FlyFish library
add_library(FlyFish STATIC
    src/FlyFish.cpp
)
target_include_directories(FlyFish
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 2) Optional: compile with C++23, enforce sane options
target_compile_features(FlyFish PUBLIC cxx_std_23)
set_target_properties(FlyFish PROPERTIES
    CXX_EXTENSIONS OFF
)

add_subdirectory(tests)   # this folder’s CMakeLists pulls in GoogleTest and registers 'FlyFish_Test'