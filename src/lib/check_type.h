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

#ifndef CHECK_TYPE_H_INCLUDED_DF7HTRDE
#define CHECK_TYPE_H_INCLUDED_DF7HTRDE


#define detail_static_assert(expr)                                            \
    ((void *)(struct                                                          \
              {                                                               \
                  _Static_assert((expr), "incompatible types");               \
              } *) 0 != (void *)0)


#define check_types_match(expr1, expr2)                                       \
    detail_static_assert(_Generic((expr1), typeof(expr2) : 1, default : 0))


#define check_type(expr, type)                                                \
    detail_static_assert(_Generic((expr), type : 1, default : 0))


#endif // include guard

