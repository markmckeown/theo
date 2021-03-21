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
#ifndef CELL_DIR_OVERFLOW_H
#define CELL_DIR_OVERFLOW_H

#include <stdint.h>
#include <stdbool.h>

#include "theo/checksum.h"
#include "theo/cell_dir_entry.h"

struct cell_dir_overflow {
	uint32_t overflow_count :16;
	uint32_t padding :16;
};


void cell_dir_overflow_init(struct cell_dir_overflow *cell_dir_overflow);

uint32_t cell_dir_overflow_add(struct cell_dir_overflow *cell_dir_overflow, char *buffer_top,
		struct cell_dir_entry *cell_dir_entry);

bool cell_dir_overflow_remove(struct cell_dir_overflow *cell_dir_overflow, char *buffer_top,
		struct checksum *checksum);

bool cell_dir_overflow_get_chunk(struct cell_dir_overflow *cell_dir_overflow, char *buffer_top,
		struct checksum *checksum, struct cell_dir_entry *cell_dir_entry);

#endif

