cmake_minimum_required(VERSION 3.5...4.1)
project(FlyFish_Tests LANGUAGES CXX)

# 1) enable C++23 for tests (match your library)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 2) enable CTest integration
enable_testing()

# 3) bring in GoogleTest via FetchContent
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP ON
)

# force shared CRT on Windows, if you care
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 4) add your test executable
add_executable(FlyFish_Test
  test_main.cpp
)

# 5) point it at your FlyFish headers
target_include_directories(FlyFish_Test PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(FlyFish_Test PRIVATE
    $<$<CONFIG:Debug>:ENGINE_DEBUG>
    $<$<CONFIG:Release>:ENGINE_RELEASE>
    $<$<CONFIG:Dist>:ENGINE_DIST>
)

# 6) link against the FlyFish library (and gtest)
target_link_libraries(FlyFish_Test PRIVATE
  FlyFish      # your static lib defined in parent CMakeLists.txt
  gtest
  gtest_main
)

# 7) register with CTest
add_test(NAME FlyFishUnitTests COMMAND FlyFish_Test)
