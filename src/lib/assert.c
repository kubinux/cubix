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

#include <lib/assert.h>
#include <lib/printf.h>

void _assert(const char *cond, const char *msg, const char *file, int line)
{
    printf("FATAL: ");
    printf("assertion %s failed ", cond);
    if (msg)
    {
        printf("(%s) ", msg);
    }
    printf("at %s:%d\n", file, line);
    while (1)
    {
        __asm__ __volatile__("cli\nhlt" ::: "memory");
    }
}

