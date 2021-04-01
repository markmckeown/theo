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
#include <string.h>

#include "theo/cell_body_entry.h"

void cell_body_entry_init(struct cell_body_entry *cell_body_entry)
{
	memset(cell_body_entry, 0, sizeof(struct cell_body_entry));
	return;
}

bool cell_body_entry_free(struct cell_body_entry *cell_body_entry)
{
	return (cell_body_entry->free_space == 1);
}

bool cell_body_entry_top(struct cell_body_entry *cell_body_entry)
{
	return (cell_body_entry->top_entry == 1);
}
