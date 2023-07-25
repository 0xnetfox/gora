#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include "parser.h"

struct p_token {
    struct token *token;
    size_t       chr_consumed;
};

bool is_whitespace(char chr) {
  return chr == ' ' || chr == '\t' || chr == '\n';
}

// TODO :: netfox :: find better name
struct p_token parse_internal(char *stream, char lexeme) {
    struct p_token res;

    if (is_whitespace(lexeme)) {
        res.token        = NULL;
        res.chr_consumed = 1;

        return res;
    }

    res.token        = NULL;
    res.chr_consumed = 1;
    return res;
}

struct token* gora_parse(char *stream) {
    char *start = stream;

    int  idx;
    char lexeme;
    while ((lexeme = stream[idx]) != '\0') {
        struct p_token tkn = parse_internal(stream, lexeme);
        idx += tkn.chr_consumed;
    }

    printf("finalized\n");
}
