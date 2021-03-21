/**
    Copyright 2021 WANdisco

    This file is part of Libtheo.

    Libtheo is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
*/
/*
 * xmalloc.c
 *
 */

#include <stdlib.h>

#include "msys.h"

void *xmalloc(size_t size)
{
	void *ptr = 0;

	ptr = malloc(size);
	ALWAYS(ptr);

	return ptr;
}

void xfree(void *ptr)
{
	ALWAYS(ptr);
	free(ptr);
	return;
}
