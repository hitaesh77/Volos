cmake_minimum_required(VERSION 3.10)
project(Volos)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Add the main library
add_library(GreeksCalculatorLib src/GreeksCalculator.cpp)

# Add the test executable
add_executable(greek tests/TestGreekCalculator.cpp)
target_link_libraries(greek GreeksCalculatorLib)