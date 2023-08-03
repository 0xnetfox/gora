cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

project(gora VERSION 0.1)
enable_testing()

add_executable(
    gora
    src/main.c
    src/lexer.c
    src/lexer.h
    src/lib/fsm.h
    src/lib/fsm.c
    src/lib/list.h
    src/lib/list.c
)

### tests
add_subdirectory(./tests/)
