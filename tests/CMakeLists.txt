add_executable(
    fsm_test_app
    ./fsm_tests.c
    ../lib/unity/unity.c
)

add_executable(
    lexer_test_app
    ./lexer_tests.c
    ../src/lib/fsm.c
    ../src/lib/list.c
    ../lib/unity/unity.c
)

add_test(fsm_test fsm_test_app)
add_test(lexer_test lexer_test_app)
