#ifndef GORA_PARSER
#define GORA_PARSER

#include <stdint.h>
#include "gora_list.h"

#define GORA_ALPH_DIGIT ((char *) "0123456789\0")

/// Indicates the valid types for tokens
enum TokenType {
    GORA_TT_IDENTIFIER,
    GORA_TT_LITERAL,
    GORA_TT_UNKNOWN
};

/// Represents a correctly parsed token
struct token {
    enum TokenType   type;
    char             *value;
    struct gora_list link;
};

struct gora_list* gora_parser_parse(char *stream);

void gora_parser_free_token_list(struct gora_list *lst);

#endif // GORA_PARSER
