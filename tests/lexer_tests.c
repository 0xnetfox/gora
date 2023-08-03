#include "../lib/unity/unity.h"
#include "../src/lexer.c"

void setUp() { }
void tearDown() { }

void assert_token(struct p_token tkn, enum TokenType tkn_type, char *tkn_value) {
    TEST_ASSERT_EQUAL(tkn.token->type,  tkn_type);
    TEST_ASSERT_EQUAL_STRING(tkn.token->value, tkn_value);

    free(tkn.token->value);
    free(tkn.token);
}

void test_st88_tokenize_number_literals() {
    assert_token(parse_internal("3",        '3'), GORA_TT_LITERAL, "3");
    assert_token(parse_internal("30.45",    '3'), GORA_TT_LITERAL, "30.45");
    assert_token(parse_internal("-3",       '-'), GORA_TT_LITERAL, "-3");
    assert_token(parse_internal("0.005",    '0'), GORA_TT_LITERAL, "0.005");
    assert_token(parse_internal("-14.0",    '-'), GORA_TT_LITERAL, "-14.0");
    assert_token(parse_internal("13772",    '1'), GORA_TT_LITERAL, "13772");
    assert_token(parse_internal("8r377",    '8'), GORA_TT_LITERAL, "8r377");
    assert_token(parse_internal("8r153",    '8'), GORA_TT_LITERAL, "8r153");
    assert_token(parse_internal("8r34.1",   '8'), GORA_TT_LITERAL, "8r34.1");
    assert_token(parse_internal("8r-37",    '8'), GORA_TT_LITERAL, "8r-37");
    assert_token(parse_internal("16r106",   '1'), GORA_TT_LITERAL, "16r106");
    assert_token(parse_internal("16rFF",    '1'), GORA_TT_LITERAL, "16rFF");
    assert_token(parse_internal("16rAC.DC", '1'), GORA_TT_LITERAL, "16rAC.DC");
    assert_token(parse_internal("16r1.C",   '1'), GORA_TT_LITERAL, "16r1.C");
    assert_token(parse_internal("1.586e5",  '1'), GORA_TT_LITERAL, "1.586e5");
    assert_token(parse_internal("1.586e-3", '1'), GORA_TT_LITERAL, "1.586e-3");
    assert_token(parse_internal("8r3e2",    '8'), GORA_TT_LITERAL, "8r3e2");
    assert_token(parse_internal("2r11e6",   '2'), GORA_TT_LITERAL, "2r11e6");
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_st88_tokenize_number_literals);
    UNITY_END();
}
