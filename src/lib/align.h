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

#ifndef ALIGN_H_INCLUDED_LOANUUW5
#define ALIGN_H_INCLUDED_LOANUUW5

#include <stdint.h>


#define ALIGN(value, alignment)                                               \
    _ALIGN_MASK(value, (typeof(value)(alignment) - 1))


#define _ALIGN_MASK(value, mask) (((value) + (mask)) & (~(mask)))


#endif // include guard

