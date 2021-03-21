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
#ifndef CELL_BODY_H
#define CELL_BODY_H

#include <stdint.h>

struct cell;
struct chunk;
struct cell_dir_entry;
struct checksum;

struct cell_body {
	uint32_t top_entry_offset;
	uint32_t next_entry_offset;
};

void cell_body_init(struct cell_body *cell_body, struct cell *cell);

void cell_body_add(struct cell_body *cell_body,
		struct cell *cell,
		struct checksum *checksum,
		char *buffer,
		uint32_t buffer_size,
		struct cell_dir_entry *cell_dir_entry);


#endif
