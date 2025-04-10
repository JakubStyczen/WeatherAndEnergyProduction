cmake_minimum_required(VERSION 3.10)

project(OpcUaWeatherFetching C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
include_directories(/usr/include)
add_subdirectory(tests)

set(SERVER_SOURCES
    src/main.c
    src/log.c
    src/opc_ua_server.c
    src/env_loader.c
    src/sections.c
)

set(CLIENT_SOURCES
    src/main_client.c
    src/log.c
    src/opc_ua_client.c
    src/weather_thread.c
    src/env_loader.c
    src/sections.c
    src/weather_condition.c
)

add_executable(server_opc_ua ${SERVER_SOURCES})

add_executable(client_opc_ua ${CLIENT_SOURCES})

target_link_libraries(server_opc_ua open62541)
target_link_libraries(client_opc_ua open62541 curl cjson)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

find_package(Doxygen REQUIRED)

if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.out)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()


