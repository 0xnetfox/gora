#ifndef GORA_FSM_H
#define GORA_FSM_H

#include <stdint.h>
#include <stdbool.h>

struct fsm {
    uint8_t            i_state;
    uint8_t           *f_states;
    struct transition *transitions;
};

struct transition {
    char*   s;
    uint8_t qi;
    uint8_t qn;
};

bool verify(struct fsm *fsm);

#endif // GORA_FSM_H
