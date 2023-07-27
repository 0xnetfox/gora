cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

project(gora VERSION 0.1)
enable_testing()

add_executable(
    gora
    src/main.c
    src/fsm.h
    src/fsm.c
    src/parser.c
    src/parser.h
    src/gora_list.h
    src/gora_list.c
)

### tests

add_executable(
    fsm_test_app
    src/fsm.c
    tests/dumb.c
    lib/unity/unity.c
)

add_test(fsm_test fsm_test_app)
