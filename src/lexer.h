#ifndef GORA_LEXER_H
#define GORA_LEXER_H

#include "lib/list.h"
#include <stdint.h>

/// Indicates the valid types for tokens
enum TokenType {
    GORA_TT_IDENTIFIER,
    GORA_TT_LITERAL,

    GORA_TT_SYM_POUND,
    GORA_TT_SYM_PAR_OPEN,
    GORA_TT_SYM_PAR_CLOSE,

    GORA_TT_UNKNOWN
};

/// Represents a correctly parsed token
struct token {
    enum TokenType   type;
    char*            value;
    struct gora_list link;
};

struct gora_list* gora_lexer_parse(char* stream);

void gora_lexer_free_token_list(struct gora_list* lst);

#endif // GORA_LEXER_H
