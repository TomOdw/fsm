cmake_minimum_required(VERSION 4.0)

project(fsm_test C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE ON)

file(GLOB SOURCES 
    ./../fsm.c
    src/fsm_test.c
    )

add_executable(fsm_test ${SOURCES})

target_include_directories(fsm_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)