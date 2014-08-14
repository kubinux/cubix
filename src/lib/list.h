// Copyright (C) 2014 Jakub Lewandowski <jakub.lewandowski@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef LIST_H_INCLUDED_2LAQIVSN
#define LIST_H_INCLUDED_2LAQIVSN


#include <lib/check_type.h>
#include <stdbool.h>
#include <stddef.h>


struct list_node
{
    struct list_node *next;
    struct list_node *prev;
};


#define declare_list_head(head_type, elem_type, member)                       \
    struct head_type                                                          \
    {                                                                         \
        union                                                                 \
        {                                                                     \
            struct list_node node;                                            \
            elem_type *type_info;                                             \
            char(*offset)[offsetof(elem_type, member)];                       \
        };                                                                    \
    }


#define list_head_initializer(name)                                           \
    {                                                                         \
        .node = {&name.node, &name.node}                                      \
    }


#define list_init_head(head) detail_list_init_head(&(head)->node)
inline void detail_list_init_head(struct list_node *node)
{
    node->next = node;
    node->prev = node;
}


#define list_empty(head) detail_list_empty((head)->node)
inline bool detail_list_empty(struct list_node *node)
{
    return node->next == node;
}


#define detail_list_node_to_elem(head, node)                                  \
    ((detail_list_type(head) *)((void *)node - detail_list_offset(head)))


#define detail_list_node_to_elem_or_null(head, n)                             \
    ((n) == &(head)->node ? NULL : detail_list_node_to_elem((head), (n)))


#define detail_list_elem_to_node(head, elem)                                  \
    ((struct list_node *)((void *)(elem) + detail_list_offset(head) +         \
                          check_types_match((elem), (head)->type_info)))


#define list_first(head)                                                      \
    detail_list_node_to_elem_or_null((head), (head)->node.next)


#define list_last(head)                                                       \
    detail_list_node_to_elem_or_null((head), (head)->node.prev)


#define list_next(head, elem)                                                 \
    detail_list_node_to_elem_or_null(                                         \
        (head), detail_list_elem_to_node((head), (elem))->next)


#define list_prev(head, elem)                                                 \
    detail_list_node_to_elem_or_null(                                         \
        (head), detail_list_elem_to_node((head), (elem))->prev)


#define list_insert(head, pos, elem)                                          \
    detail_list_insert(detail_list_elem_to_node((head), (pos)),               \
                       detail_list_elem_to_node((head), (elem)))
inline void detail_list_insert(struct list_node *pos, struct list_node *node)
{
    node->next = pos->next;
    node->prev = pos;
    node->next->prev = node;
    pos->next = node;
}


#define list_append(head, elem)                                               \
    detail_list_insert(                                                       \
        (head)->node.prev,                                                    \
        detail_list_elem_to_node((head), (elem)))


#define list_prepend(head, elem)                                              \
    detail_list_insert(                                                       \
        &(head)->node,                                                        \
        detail_list_elem_to_node((head), (elem)))


#define list_remove(head, elem)                                               \
    detail_list_remove(detail_list_elem_to_node((head), (elem)))
inline void detail_list_remove(struct list_node *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = NULL;
    node->prev = NULL;
}


#define detail_list_type(head) typeof(*(head)->type_info)


#define detail_list_offset(head) sizeof(*(head)->offset)


#define detail_list_first_elem(head)                                          \
    detail_list_node_to_elem((head), (head)->node.next)


#define detail_list_next_elem(head, elem)                                     \
    detail_list_node_to_elem((head),                                          \
                             detail_list_elem_to_node((head), elem)->next)


#define list_for_each(head, iter)                                             \
    for (iter = detail_list_first_elem(head);                                 \
         detail_list_elem_to_node((head), iter) != &(head)->node;             \
         iter = detail_list_next_elem((head), iter))


#define list_for_each_safe(head, iter)                                        \
    for (detail_list_type(head) *_next = detail_list_next_elem(               \
             (head), (iter = detail_list_first_elem(head)));                  \
         detail_list_elem_to_node((head), iter) != &(head)->node;             \
         iter = _next, _next = detail_list_next_elem((head), iter))


#endif // include guard

