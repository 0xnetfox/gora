#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "fsm.h"

// TODO :: netfox :: write test for this
bool is_valid(struct fsm *fsm, uint8_t state) {
    int i = 0;
    uint8_t f_state;
    while ((f_state = fsm->f_states[i++]) != GORA_FSM_NULL_STATE) {
        if (f_state == state) {
            return true;
        }
    }

    return false;
}

uint8_t next_state(struct fsm *fsm, uint8_t state, char input) {
    for (int i = 0; fsm->transitions[i].i_state != GORA_FSM_NULL_STATE; i++) {
        struct transition *t = &fsm->transitions[i];
        
        if (t->i_state != state)
            continue;

        if (t->syms(input)) {
            return t->n_state;
        }
    }

    return GORA_FSM_NULL_STATE;
}

struct fsm_result
solve(struct fsm *fsm, uint8_t state, char *stream, size_t stream_off) {
    char input = stream[stream_off];

    if (input == '\0')
        goto ret;

    uint8_t next = next_state(fsm, state, input);
    if (next != GORA_FSM_NULL_STATE) {
        return solve(fsm, next, stream, stream_off + 1);
    }

ret:
    return (struct fsm_result) {
        .state           = state,
        .valid           = is_valid(fsm, state),
        .stream_consumed = stream_off,
    };
}

struct fsm_result
gora_fsm_solve(struct fsm *fsm, char *stream) {
    return solve(fsm, fsm->i_state, stream, 0);
}
