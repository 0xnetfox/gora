#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "fsm.h"
#include "gora_list.h"
#include "parser.h"

#define GORA_PARSER_CAP_ALPHA_NUM GORA_FSM_ALPH_B10_DIGIT GORA_FSM_ALPH_CAP_LETTERS

// TODO :: netfox :: write tests owo
static struct fsm st88_number_fsm = {
    .i_state     = 1,
    .f_states    = (uint8_t[]) { 2, 4, 6, 9, 12, 14, GORA_FSM_NULL_STATE },
    .transitions = (struct transition[]) {
      { .i_state = 1,  .n_state = 2,  .syms = GORA_FSM_ALPH_B10_DIGIT   },
      { .i_state = 1,  .n_state = 3,  .syms = "-"                       },
      { .i_state = 2,  .n_state = 2,  .syms = GORA_FSM_ALPH_B10_DIGIT   },
      { .i_state = 2,  .n_state = 5,  .syms = "."                       },
      { .i_state = 2,  .n_state = 7,  .syms = "e"                       },
      { .i_state = 2,  .n_state = 10, .syms = "r"                       },
      { .i_state = 3,  .n_state = 4,  .syms = GORA_FSM_ALPH_B10_DIGIT   },
      { .i_state = 4,  .n_state = 4,  .syms = GORA_FSM_ALPH_B10_DIGIT   },
      { .i_state = 4,  .n_state = 5,  .syms = "."                       },
      { .i_state = 4,  .n_state = 7,  .syms = "e"                       },
      { .i_state = 5,  .n_state = 6,  .syms = GORA_FSM_ALPH_B10_DIGIT   },
      { .i_state = 6,  .n_state = 6,  .syms = GORA_FSM_ALPH_B10_DIGIT   },
      { .i_state = 6,  .n_state = 7,  .syms = "e"                       },
      { .i_state = 7,  .n_state = 8,  .syms = "-"                       },
      { .i_state = 7,  .n_state = 9,  .syms = GORA_FSM_ALPH_B10_DIGIT   },
      { .i_state = 8,  .n_state = 9,  .syms = GORA_FSM_ALPH_B10_DIGIT   },
      { .i_state = 9,  .n_state = 9,  .syms = GORA_FSM_ALPH_B10_DIGIT   },
      { .i_state = 10, .n_state = 12, .syms = GORA_PARSER_CAP_ALPHA_NUM },
      { .i_state = 10, .n_state = 11, .syms = "-"                       },
      { .i_state = 11, .n_state = 12, .syms = GORA_PARSER_CAP_ALPHA_NUM },
      { .i_state = 12, .n_state = 12, .syms = GORA_PARSER_CAP_ALPHA_NUM },
      { .i_state = 12, .n_state = 13, .syms = "."                       },
      { .i_state = 12, .n_state = 7,  .syms = "e"                       },
      { .i_state = 13, .n_state = 14, .syms = GORA_PARSER_CAP_ALPHA_NUM },
      { .i_state = 14, .n_state = 14, .syms = GORA_PARSER_CAP_ALPHA_NUM },
      { .i_state = 14, .n_state = 7,  .syms = "e"                       },
      GORA_FSM_NULL_TRANSITION
    },
};

// ALG to solve fsm
// start on f_n1, fetch character from stream
// if character has a transition from f_n1 -> f_n2, recurse
// otherwise, check if f_n1 is an end state

struct p_token {
    struct token *token;
    size_t       chr_consumed;
};

bool is_whitespace(char chr) {
  return chr == ' ' || chr == '\t' || chr == '\n';
}

// TODO :: this may fail as - can be a value by itself in an expr
//         .e.g. 32 - 15
bool is_st88_numeric(char chr) {
    return (chr >= '0' && chr <= '9') || chr == '-';
}

int try_parse_numeric(char *stream, struct p_token *p_token) {
    struct fsm_result fsm_res = gora_fsm_solve(&st88_number_fsm, stream);

    if (!fsm_res.valid)
        return 1;

    p_token->token = malloc(sizeof(struct token));

    p_token->token->value = malloc(fsm_res.stream_consumed + 1 * sizeof(char));
    strncpy(p_token->token->value, stream, fsm_res.stream_consumed);
    p_token->token->value[fsm_res.stream_consumed] = '\0';

    p_token->token->type = GORA_TT_LITERAL;
    p_token->chr_consumed = fsm_res.stream_consumed;
    gora_list_init(&p_token->token->link);

    return 0;
}

// TODO :: netfox :: find better name
struct p_token parse_internal(char *stream, char lexeme) {
    struct p_token p_token;

    if (is_whitespace(lexeme)) {
        p_token.token        = NULL;
        p_token.chr_consumed = 1;

        return p_token;
    }

    if (is_st88_numeric(lexeme)) {
        int res = try_parse_numeric(stream, &p_token);
        if (res == 0)
            return p_token;
    }

    p_token.token        = malloc(sizeof(struct token));
    p_token.token->type  = GORA_TT_UNKNOWN;

    p_token.token->value    = malloc(2 * sizeof(char));
    p_token.token->value[0] = lexeme;
    p_token.token->value[1] = '\0';

    gora_list_init(&p_token.token->link);

    p_token.chr_consumed = 1;

    return p_token;
}

struct gora_list* gora_parser_parse(char *stream) {
    int  idx = 0;
    char lexeme;

    struct gora_list *token_lst = malloc(sizeof(struct gora_list)); 
    struct gora_list *token_lst_tail = token_lst;
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

void gora_parser_free_token_list(struct gora_list *lst) {
    struct token *curr, *next; 

    gora_list_foreach_safe(curr, next, lst, link) {
        free(curr->value);
        free(curr);
    }
}
