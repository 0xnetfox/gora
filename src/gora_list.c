#include "gora_list.h"
#include <stddef.h>

void gora_list_init(struct gora_list* lst)
{
    lst->prev = lst;
    lst->next = lst;
}

void gora_list_insert(struct gora_list* lst, struct gora_list* elm)
{
    elm->prev       = lst;
    elm->next       = lst->next;
    lst->next       = elm;
    elm->next->prev = elm;
}
