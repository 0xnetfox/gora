#ifndef GORA_LIST
#define GORA_LIST

/**
 * defines a double-linked structure and methods to interact with it.
 *
 * Largely inspired by wayland's wl_list.
 * https://gitlab.freedesktop.org/wayland/wayland/-/blob/main/src/wayland-util.h
 */

struct gora_list {
    struct gora_list* prev;
    struct gora_list* next;
};

void gora_list_init(struct gora_list* lst);

void gora_list_insert(struct gora_list* lst, struct gora_list* elm);

#define gora_container_of(p_item, type, member) \
    (__typeof__(type))((char*)(p_item)-offsetof(__typeof__(*type), member))

#define gora_list_foreach_safe(pos, tmp, head, member)             \
    for (pos = gora_container_of((head)->next, pos, member),       \
        tmp  = gora_container_of((pos)->member.next, tmp, member); \
         &pos->member != (head);                                   \
         pos = tmp,                                                \
        tmp  = gora_container_of(pos->member.next, tmp, member))

#endif // GORA_LIST
