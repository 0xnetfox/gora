#ifndef GORA_PARSER
#define GORA_PARSER

#include "gora_list.h"

/// Indicates the valid types for tokens
enum TokenType {
    GORA_TT_IDENTIFIER,
    GORA_TT_LITERAL,
    GORA_TT_TEMPORAL
};

/// Represents a correctly parsed token
struct token {
    enum TokenType   type;
    char             *value;
    struct gora_list link;
};

struct gora_list* gora_parser_parse(char *stream);

#endif // GORA_PARSER
