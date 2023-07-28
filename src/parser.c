#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "gora_list.h"
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

    res.token        = malloc(sizeof(struct token));
    res.token->type  = GORA_TT_TEMPORAL;

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
        struct p_token parsed = parse_internal(stream, lexeme);

        if (parsed.token != NULL) {
            gora_list_insert(token_lst_tail, &parsed.token->link);
            token_lst_tail = &parsed.token->link;
            printf("parsing: %s, %p, %p\n", parsed.token->value, parsed.token, &parsed.token->link);
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
