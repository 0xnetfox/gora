#include "../lib/unity/unity.h"
#include "../src/lib/fsm.c"

void setUp() { }
void tearDown() { }

bool syms_test_num(char sym)
{
    return sym >= '0' && sym <= '9';
}

GORA_FSM_TEST_CHAR(syms_test_dot, '.')
GORA_FSM_TEST_CHAR(syms_test_hypen, '-')

// clang-format off

/// FSM for tests, parses any number with the following format:
/// 1, 1.23, -1, -1.23
static struct fsm test_fsm_num = {
    .i_state     = 1,
    .f_states    = (uint8_t[]) { 2, 5, GORA_FSM_NULL_STATE },
    .transitions = (struct transition[]) {
      { .i_state = 1, .n_state = 2, .syms = syms_test_num   },
      { .i_state = 1, .n_state = 3, .syms = syms_test_hypen },
      { .i_state = 2, .n_state = 2, .syms = syms_test_num   },
      { .i_state = 2, .n_state = 4, .syms = syms_test_dot   },
      { .i_state = 3, .n_state = 2, .syms = syms_test_num   },
      { .i_state = 4, .n_state = 5, .syms = syms_test_num   },
      { .i_state = 5, .n_state = 5, .syms = syms_test_num   },
      GORA_FSM_NULL_TRANSITION
    },
};

// clang-format on

void test_next_gives_correct_state()
{
    uint8_t n_state_1 = next_state(&test_fsm_num, 1, '8');
    uint8_t n_state_2 = next_state(&test_fsm_num, 1, '-');
    uint8_t n_state_3 = next_state(&test_fsm_num, 2, '8');

    TEST_ASSERT_EQUAL(n_state_1, 2);
    TEST_ASSERT_EQUAL(n_state_2, 3);
    TEST_ASSERT_EQUAL(n_state_3, 2);
}

void test_next_fails_with_unvalid_state()
{
    uint8_t n_state_1 = next_state(&test_fsm_num, 1, '/');
    uint8_t n_state_2 = next_state(&test_fsm_num, 4, '-');

    TEST_ASSERT_EQUAL(n_state_1, GORA_FSM_NULL_STATE);
    TEST_ASSERT_EQUAL(n_state_2, GORA_FSM_NULL_STATE);
}

void test_solve_finds_end_state()
{
    struct fsm_result end_state_1 = gora_fsm_solve(&test_fsm_num, "123");
    struct fsm_result end_state_2 = gora_fsm_solve(&test_fsm_num, "123.15");
    struct fsm_result end_state_3 = gora_fsm_solve(&test_fsm_num, "-123");
    struct fsm_result end_state_4 = gora_fsm_solve(&test_fsm_num, "-123.15");

    TEST_ASSERT_TRUE(end_state_1.valid);
    TEST_ASSERT_TRUE(end_state_2.valid);
    TEST_ASSERT_TRUE(end_state_3.valid);
    TEST_ASSERT_TRUE(end_state_4.valid);

    TEST_ASSERT_EQUAL(end_state_1.state, 2);
    TEST_ASSERT_EQUAL(end_state_1.stream_consumed, 3);
    TEST_ASSERT_EQUAL(end_state_2.state, 5);
    TEST_ASSERT_EQUAL(end_state_2.stream_consumed, 6);
    TEST_ASSERT_EQUAL(end_state_3.state, 2);
    TEST_ASSERT_EQUAL(end_state_3.stream_consumed, 4);
    TEST_ASSERT_EQUAL(end_state_4.state, 5);
    TEST_ASSERT_EQUAL(end_state_4.stream_consumed, 7);
}

void test_solve_returns_initial_state_on_invalid_input()
{
    struct fsm_result end_state_1 = gora_fsm_solve(&test_fsm_num, "abc");

    TEST_ASSERT_FALSE(end_state_1.valid);
    TEST_ASSERT_EQUAL(end_state_1.state, 1);
    TEST_ASSERT_EQUAL(end_state_1.stream_consumed, 0);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_next_gives_correct_state);
    RUN_TEST(test_next_fails_with_unvalid_state);
    RUN_TEST(test_solve_finds_end_state);
    RUN_TEST(test_solve_returns_initial_state_on_invalid_input);
    UNITY_END();
}
