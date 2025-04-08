enable_language(CXX)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})
include_directories(/usr/include)
find_package(GTest REQUIRED)

set(LOG_SOURCES
    ${PROJECT_SOURCE_DIR}/src/log.c
)

add_executable(run_tests test_main.cpp test_env_loader.cpp test_sections.cpp test_weather_condition.cpp)
target_sources(run_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/src/weather_condition.c ${LOG_SOURCES}
)
target_sources(run_tests PRIVATE ${CMAKE_SOURCE_DIR}/src/env_loader.c ${LOG_SOURCES})
target_sources(run_tests PRIVATE ${CMAKE_SOURCE_DIR}/src/sections.c ${LOG_SOURCES})
target_link_libraries(run_tests ${GTEST_LIBRARIES} ${GMOCK_INCLUDE_DIRS} curl cjson pthread)