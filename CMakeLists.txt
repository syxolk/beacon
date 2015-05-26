project(beacon)
cmake_minimum_required(VERSION 2.8)

set(SOURCES
    src/main.cpp
    src/common.cpp
    src/sender.cpp
    src/receiver.cpp
)

set(HEADERS
    include/beacon/common.h
    include/beacon/sender.h
    include/beacon/receiver.h
)

include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-W -Wall -Werror -pedantic -std=c++11")
