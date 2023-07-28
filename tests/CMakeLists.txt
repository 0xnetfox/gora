add_executable(
    fsm_test_app
    ../src/fsm.c
    ./dumb.c
    ../lib/unity/unity.c
)

add_test(fsm_test fsm_test_app)
