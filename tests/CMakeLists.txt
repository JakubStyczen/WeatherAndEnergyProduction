enable_language(CXX)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})
include_directories(/usr/include)
include_directories(/usr/local/include)
find_package(GTest REQUIRED)

add_executable(run_tests test_main.cpp test_env_loader.cpp test_sections.cpp test_weather_condition.cpp)
target_sources(run_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/src/weather_condition.c
)
target_sources(run_tests PRIVATE ${CMAKE_SOURCE_DIR}/src/env_loader.c)
target_sources(run_tests PRIVATE ${CMAKE_SOURCE_DIR}/src/sections.c)
target_link_libraries(run_tests ${GTEST_LIBRARIES} ${GMOCK_INCLUDE_DIRS} curl cjson pthread)