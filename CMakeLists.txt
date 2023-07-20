cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

project(gora VERSION 0.1)
add_executable(
    gora
    src/main.c
    src/parser.c
    src/parser.h
)
