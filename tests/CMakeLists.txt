add_executable(
    fsm_test_app
    ./fsm_tests.c
    ../lib/unity/unity.c
)

add_test(fsm_test fsm_test_app)
