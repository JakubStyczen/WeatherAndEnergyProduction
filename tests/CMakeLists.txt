enable_language(CXX)

find_package(GTest REQUIRED)

add_executable(run_tests test_main.cpp)

target_link_libraries(run_tests ${GTEST_LIBRARIES} pthread)