cmake_minimum_required(VERSION 3.10)

project(OpcUaWeatherFetching C)

set(CMAKE_C_STANDARD 17)

include_directories(include)

set(SERVER_SOURCES
    src/main.c
    src/opc_ua_server.c
    src/env_loader.c
    src/sections.c
)

set(CLIENT_SOURCES
    src/main_client.c
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
