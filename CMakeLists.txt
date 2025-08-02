cmake_minimum_required(VERSION 3.00)

project(c-programming C)

add_executable(main main.c)

add_custom_target(run
    COMMAND main
    DEPENDS main
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
