#ifndef GORA_FSM_H
#define GORA_FSM_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define GORA_FSM_NULL_STATE 0
#define GORA_FSM_NULL_TRANSITION      \
    { .i_state = GORA_FSM_NULL_STATE, \
      .n_state = GORA_FSM_NULL_STATE, \
      .syms = NULL }

#define GORA_FSM_TEST_CHAR(name, chr) \
    static inline bool name(char sym) {      \
        return chr == sym;     \
    }

struct fsm {
    uint8_t            i_state;
    uint8_t           *f_states;
    struct transition *transitions;
};

struct transition {
    uint8_t i_state;
    uint8_t n_state;

    bool (*syms)(char);
};

struct fsm_result {
    bool    valid;
    uint8_t state;
    size_t  stream_consumed;
};

struct fsm_result gora_fsm_solve(struct fsm *fsm, char *stream);

#endif // GORA_FSM_H
