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

#include "theo/cell_dir_entry.h"

void cell_dir_entry_init(struct cell_dir_entry *cell_dir_entry)
{
	memset(cell_dir_entry, 0, sizeof(struct cell_dir_entry));
	checksum_init(&cell_dir_entry->checksum);
	return;
}

void cell_dir_entry_copy(struct cell_dir_entry *to, struct cell_dir_entry *from)
{
	memcpy(to, from, sizeof(struct cell_dir_entry));
	return;
}

void cell_dir_entry_set(struct cell_dir_entry *cell_dir_entry,
			struct checksum *checksum, uint32_t offset)
{
	memcpy(&cell_dir_entry->checksum, checksum, sizeof(struct checksum));
	cell_dir_entry->set = 1;
	cell_dir_entry->offset = offset;
	return;
}
