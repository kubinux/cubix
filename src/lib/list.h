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


#include <stdbool.h>
#include <stddef.h>


struct list_node
{
    struct list_node *next;
    struct list_node *prev;
};


struct list_head
{
    struct list_node node;
};


inline void list_head_init(struct list_head *head)
{
    head->node.next = &head->node;
    head->node.prev = &head->node;
}


#define LIST_HEAD_INITIALIZER(head)                                           \
    {                                                                         \
        {                                                                     \
            &head.node, &head.node                                            \
        }                                                                     \
    }


inline bool list_empty(struct list_head *head)
{
    return head->node.next == &head->node;
}


inline void list_insert(struct list_node *pos, struct list_node *node)
{
    node->next = pos->next;
    node->prev = pos;
    node->next->prev = node;
    pos->next = node;
}


inline void list_append(struct list_head *head, struct list_node *node)
{
    list_insert(head->node.prev, node);
}


inline void list_prepend(struct list_head *head, struct list_node *node)
{
    list_insert(&head->node, node);
}


inline void list_remove(struct list_node *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = NULL;
    node->prev = NULL;
}


#define container_of(ptr, type, member)                                       \
    ((type *)((char *)(ptr) - offsetof(type, member)))


#define list_entry(node, type, member) container_of((node), type, member)


#define detail_list_first(head, type, member)                                 \
    list_entry((head)->node.next, type, member)


#define detail_list_next(node, type, member)                                  \
    list_entry((node)->next, type, member)


#define list_for_each(head, iter, member)                                     \
    for (iter = detail_list_first(head, typeof(*iter), member);               \
         &iter->member != &(head)->node;                                             \
         iter = detail_list_next(&iter->member, typeof(*iter), member))


#endif // include guard

