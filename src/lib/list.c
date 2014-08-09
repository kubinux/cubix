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

#include <lib/list.h>


extern void list_head_init(struct list_head *head);


extern bool list_empty(struct list_head *head);


extern void list_insert(struct list_node *pos, struct list_node *node);


extern void list_append(struct list_head *head, struct list_node *node);


extern void list_prepend(struct list_head *head, struct list_node *node);


extern void list_remove(struct list_node *node);

