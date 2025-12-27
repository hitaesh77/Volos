# Create the test executable
add_executable(unit_tests test_bsm_greeks.cpp)

# Link the test to your logic AND the GTest framework
# GTest::gtest_main provides a built-in main() function for you
target_link_libraries(unit_tests PRIVATE core_lib GTest::gtest_main)

# Standard GoogleTest discovery
include(GoogleTest)
gtest_discover_tests(unit_tests)