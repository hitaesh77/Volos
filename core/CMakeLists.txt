cmake_minimum_required(VERSION 3.14)
project(VolosCore VERSION 1.0 LANGUAGES CXX)

# Standard C++ settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Download Dependencies (GoogleTest)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 2. Define the Library
# We include all .cpp files in src/bsm
add_library(core_lib src/bsm/bsm_greeks.cpp)

# 3. Handle Headers
# 'PUBLIC' means anyone linking to core_lib automatically sees the 'include' folder
target_include_directories(core_lib PUBLIC include)

# 4. Add Subdirectories
enable_testing()
add_subdirectory(tests)
# add_subdirectory(benchmarks) # Uncomment once benchmarks are ready