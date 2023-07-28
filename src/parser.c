#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "fsm.h"
#include "gora_list.h"
#include "parser.h"

#define GORA_FSM_FSTATES_END 0

static struct fsm num_fsm = {
    .i_state     = 1,
    .f_states    = (uint8_t[]) { 2, 5, GORA_FSM_INVALID_STATE },
    .transitions = (struct transition[]) {
      { .i_state = 1, .n_state = 2, .syms = GORA_FSM_ALPH_DIGIT },
      { .i_state = 1, .n_state = 3, .syms = "-"                 },
      { .i_state = 2, .n_state = 2, .syms = GORA_FSM_ALPH_DIGIT },
      { .i_state = 2, .n_state = 4, .syms = "."                 },
      { .i_state = 3, .n_state = 2, .syms = GORA_FSM_ALPH_DIGIT },
      { .i_state = 4, .n_state = 5, .syms = GORA_FSM_ALPH_DIGIT },
      { .i_state = 5, .n_state = 5, .syms = GORA_FSM_ALPH_DIGIT },
      GORA_FSM_INVALID_TRANSITION
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

bool is_number(char chr) {
    return chr >= 0x30 && chr <= 0x39;
}

char *gora_strncpy(char *dest, char *src, size_t count) {
    // ensure there's enough space on dest to null-terminate it
    assert(count < sizeof(dest));

    strncpy(dest, src, count);
    dest[sizeof(dest) - 1] = '\0';

    return dest;
}

// TODO :: netfox :: find better name
struct p_token parse_internal(char *stream, size_t stream_off, char lexeme) {
    struct p_token res;
    char *cursor = stream + stream_off;

    if (is_whitespace(lexeme)) {
        res.token        = NULL;
        res.chr_consumed = 1;

        return res;
    }

    if (is_number(lexeme) || lexeme == '-') {
        struct fsm_result fsm_res =
            gora_fsm_solve(&num_fsm, cursor);

        if (fsm_res.valid) {
            res.token = malloc(sizeof(struct token));
            res.token->value = malloc(fsm_res.stream_consumed + 1 * sizeof(char));

            res.token->type = GORA_TT_LITERAL;
            gora_strncpy(res.token->value, cursor, fsm_res.stream_consumed);
            gora_list_init(&res.token->link);
            res.chr_consumed = fsm_res.stream_consumed;

            return res;
        }
    }

    res.token        = malloc(sizeof(struct token));
    res.token->type  = GORA_TT_UNKNOWN;

    res.token->value    = malloc(2 * sizeof(char));
    res.token->value[0] = lexeme;
    res.token->value[1] = '\0';

    gora_list_init(&res.token->link);

    res.chr_consumed = 1;

    return res;
}

struct gora_list* gora_parser_parse(char *stream) {
    int  idx;
    char lexeme;

    struct gora_list *token_lst = malloc(sizeof(struct gora_list)); 
    struct gora_list *token_lst_tail = token_lst;
    gora_list_init(token_lst);

    while ((lexeme = stream[idx]) != '\0') {
        struct p_token parsed = parse_internal(stream, idx, lexeme);

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
