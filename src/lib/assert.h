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

#ifndef ASSERT_H_INCLUDED_NFFUIUQL
#define ASSERT_H_INCLUDED_NFFUIUQL


void _assert(const char *cond, const char *msg, const char *file, int line);


#define ASSERT_MSG(cond, msg)                                                 \
    do                                                                        \
    {                                                                         \
        if (!(cond))                                                          \
        {                                                                     \
            _assert(#cond, msg, __FILE__, __LINE__);                          \
        }                                                                     \
    } while (0)


#define ASSERT(cond)                                                          \
    do                                                                        \
    {                                                                         \
        if (!(cond))                                                          \
        {                                                                     \
            _assert(#cond, 0, __FILE__, __LINE__);                            \
        }                                                                     \
    } while (0)


#endif // include guard

