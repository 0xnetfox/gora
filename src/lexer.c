#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/fsm.h"
#include "lib/list.h"
#include "lexer.h"

#define GORA_PARSER_CAP_ALPHA_NUM GORA_FSM_ALPH_B10_DIGIT GORA_FSM_ALPH_CAP_LETTERS

GORA_FSM_TEST_CHAR(syms_test_e, 'e')
GORA_FSM_TEST_CHAR(syms_test_r, 'r')
GORA_FSM_TEST_CHAR(syms_test_dot, '.')
GORA_FSM_TEST_CHAR(syms_test_hypen, '-')
GORA_FSM_TEST_CHAR(syms_test_pound, '#')
GORA_FSM_TEST_CHAR(syms_test_quote, '\'')
GORA_FSM_TEST_CHAR(syms_test_dollar, '$')
GORA_FSM_TEST_CHAR(syms_test_open_parenthesis, '(')
GORA_FSM_TEST_CHAR(syms_test_close_parenthesis, ')')

bool syms_test_num(char sym)
{
    return sym >= '0' && sym <= '9';
}

bool syms_test_upp_alph(char sym)
{
    return sym >= 'A' && sym <= 'Z';
}

bool syms_test_low_alph(char sym)
{
    return sym >= 'a' && sym <= 'z';
}

bool syms_test_any_visible_ascii(char sym)
{
    return sym >= 0x21 && sym <= 0x7E;
}

bool syms_test_any_string_ascii(char sym)
{
    return !syms_test_quote(sym) && sym >= 0x20 && sym <= 0x7E;
}

bool syms_test_upp_alph_num(char sym)
{
    return syms_test_upp_alph(sym) || syms_test_num(sym);
}

bool syms_test_alph(char sym)
{
    return syms_test_low_alph(sym) || syms_test_upp_alph(sym);
}

bool syms_test_alph_num(char sym)
{
    return syms_test_alph(sym) || syms_test_num(sym);
}

// clang-format off
static struct fsm st88_identifier_fsm = {
    .i_state     = 1,
    .f_states    = (uint8_t[]) { 2, GORA_FSM_NULL_STATE },
    .transitions = (struct transition[]) {
        { .i_state = 1, .n_state = 2, .syms = syms_test_alph     },
        { .i_state = 2, .n_state = 2, .syms = syms_test_alph_num },
        GORA_FSM_NULL_TRANSITION
    },
};

static struct fsm st88_string_fsm = {
    .i_state     = 1,
    .f_states    = (uint8_t[]) { 3, GORA_FSM_NULL_STATE },
    .transitions = (struct transition[]) {
        { .i_state = 1, .n_state = 2, .syms = syms_test_quote            },
        { .i_state = 2, .n_state = 2, .syms = syms_test_any_string_ascii },
        { .i_state = 2, .n_state = 3, .syms = syms_test_quote            },
        { .i_state = 3, .n_state = 2, .syms = syms_test_quote            },
        GORA_FSM_NULL_TRANSITION
    },
};

static struct fsm st88_character_fsm = {
    .i_state     = 1,
    .f_states    = (uint8_t[]) { 3, GORA_FSM_NULL_STATE },
    .transitions = (struct transition[]) {
        { .i_state = 1, .n_state = 2, .syms = syms_test_dollar            },
        { .i_state = 2, .n_state = 3, .syms = syms_test_any_visible_ascii },
        GORA_FSM_NULL_TRANSITION
    },
};

static struct fsm st88_number_fsm = {
    .i_state     = 1,
    .f_states    = (uint8_t[]) { 2, 4, 6, 9, 12, 14, GORA_FSM_NULL_STATE },
    .transitions = (struct transition[]) {
      { .i_state = 1,  .n_state = 2,  .syms = syms_test_num          },
      { .i_state = 1,  .n_state = 3,  .syms = syms_test_hypen        },
      { .i_state = 2,  .n_state = 2,  .syms = syms_test_num          },
      { .i_state = 2,  .n_state = 5,  .syms = syms_test_dot          },
      { .i_state = 2,  .n_state = 7,  .syms = syms_test_e            },
      { .i_state = 2,  .n_state = 10, .syms = syms_test_r            },
      { .i_state = 3,  .n_state = 4,  .syms = syms_test_num          },
      { .i_state = 4,  .n_state = 4,  .syms = syms_test_num          },
      { .i_state = 4,  .n_state = 5,  .syms = syms_test_dot          },
      { .i_state = 4,  .n_state = 7,  .syms = syms_test_e            },
      { .i_state = 5,  .n_state = 6,  .syms = syms_test_num          },
      { .i_state = 6,  .n_state = 6,  .syms = syms_test_num          },
      { .i_state = 6,  .n_state = 7,  .syms = syms_test_e            },
      { .i_state = 7,  .n_state = 8,  .syms = syms_test_hypen        },
      { .i_state = 7,  .n_state = 9,  .syms = syms_test_num          },
      { .i_state = 8,  .n_state = 9,  .syms = syms_test_num          },
      { .i_state = 9,  .n_state = 9,  .syms = syms_test_num          },
      { .i_state = 10, .n_state = 12, .syms = syms_test_upp_alph_num },
      { .i_state = 10, .n_state = 11, .syms = syms_test_hypen        },
      { .i_state = 11, .n_state = 12, .syms = syms_test_upp_alph_num },
      { .i_state = 12, .n_state = 12, .syms = syms_test_upp_alph_num },
      { .i_state = 12, .n_state = 13, .syms = syms_test_dot          },
      { .i_state = 12, .n_state = 7,  .syms = syms_test_e            },
      { .i_state = 13, .n_state = 14, .syms = syms_test_upp_alph_num },
      { .i_state = 14, .n_state = 14, .syms = syms_test_upp_alph_num },
      { .i_state = 14, .n_state = 7,  .syms = syms_test_e            },
      GORA_FSM_NULL_TRANSITION
    },
};
// clang-format on

struct p_token {
    struct token* token;
    size_t        chr_consumed;
};

bool is_whitespace(char chr)
{
    return chr == ' ' || chr == '\t' || chr == '\n';
}

// TODO :: this may fail as - can be a value by itself in an expr
//         .e.g. 32 - 15
bool is_st88_numeric(char chr)
{
    return (chr >= '0' && chr <= '9') || chr == '-';
}

bool is_st88_character(char chr)
{
    return chr == '$';
}

bool is_st88_string(char chr)
{
    return chr == '\'';
}

int alloc_token_of_char(char chr, enum TokenType type, struct p_token *p_token) {
    p_token-> token = malloc(sizeof(struct token));
    p_token->token->value = malloc(2 * sizeof(char));
    p_token->token->value[0] = chr;
    p_token->token->value[1] = '\0';

    p_token->token->type  = type;
    p_token->chr_consumed = 1;
    gora_list_init(&p_token->token->link);

    return 0;
}

int try_parse_fsm(
    struct fsm*     fsm,
    char*           stream,
    enum TokenType  type,
    struct p_token* p_token)
{
    struct fsm_result fsm_res = gora_fsm_solve(fsm, stream);

    if (!fsm_res.valid)
        return 1;

    p_token->token = malloc(sizeof(struct token));

    p_token->token->value = malloc(fsm_res.stream_consumed + 1 * sizeof(char));
    strncpy(p_token->token->value, stream, fsm_res.stream_consumed);
    p_token->token->value[fsm_res.stream_consumed] = '\0';

    p_token->token->type  = type;
    p_token->chr_consumed = fsm_res.stream_consumed;
    gora_list_init(&p_token->token->link);

    return 0;
}

// TODO :: netfox :: find better name
struct p_token parse_internal(char* stream, char lexeme)
{
    int res;
    struct p_token p_token;

    if (is_whitespace(lexeme)) {
        p_token.token        = NULL;
        p_token.chr_consumed = 1;

        return p_token;
    }

    if (is_st88_numeric(lexeme))
        res = try_parse_fsm(&st88_number_fsm, stream, GORA_TT_LITERAL, &p_token);
    else if (is_st88_character(lexeme))
        res = try_parse_fsm(&st88_character_fsm, stream, GORA_TT_LITERAL, &p_token);
    else if (is_st88_string(lexeme))
        res = try_parse_fsm(&st88_string_fsm, stream, GORA_TT_LITERAL, &p_token);
    // TODO :: netfox :: identifier should store a relation to the sym table,
    // not the literal identifier name.
    else if (syms_test_alph(lexeme))
        res = try_parse_fsm(&st88_identifier_fsm, stream, GORA_TT_IDENTIFIER, &p_token);
    // TODO :: netfox :: it may be better to write a table to pair the lexeme
    // to its token type, then we can refactor the blocks below that share the
    // same logic.
    else if (lexeme == '#')
        res = alloc_token_of_char(lexeme, GORA_TT_SYM_POUND, &p_token);
    else if (lexeme == '(')
        res = alloc_token_of_char(lexeme, GORA_TT_SYM_PAR_OPEN, &p_token);
    else if (lexeme == ')')
        res = alloc_token_of_char(lexeme, GORA_TT_SYM_PAR_CLOSE, &p_token);

    if (res == 0)
        return p_token;

    alloc_token_of_char(lexeme, GORA_TT_UNKNOWN, &p_token);
    return p_token;
}

struct gora_list* gora_lexer_parse(char* stream)
{
    int  idx = 0;
    char lexeme;

    struct gora_list* token_lst      = malloc(sizeof(struct gora_list));
    struct gora_list* token_lst_tail = token_lst;
    gora_list_init(token_lst);

    while ((lexeme = stream[idx]) != '\0') {
        struct p_token parsed = parse_internal(stream + idx, lexeme);

        if (parsed.token != NULL) {
            gora_list_insert(token_lst_tail, &parsed.token->link);
            token_lst_tail = &parsed.token->link;

            printf("Token<%u, %s>\n", parsed.token->type, parsed.token->value);
        }

        idx += parsed.chr_consumed;
    }

    return token_lst;
}

void gora_lexer_free_token_list(struct gora_list* lst)
{
    struct token *curr, *next;

    gora_list_foreach_safe(curr, next, lst, link)
    {
        free(curr->value);
        free(curr);
    }
}
