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
#ifndef CELL_DIR_ENTRY_H
#define CELL_DIR_ENTRY_H

#include "theo/checksum.h" 

// Reference for a chunk in the cell.
struct cell_dir_entry {
	// checksum of chunk
	struct checksum checksum;
	// offset into the cell for the data.
	uint32_t offset :31;
	uint32_t set    : 1;
};

void cell_dir_entry_init(struct cell_dir_entry *cell_dir_entry);

void cell_dir_entry_copy(struct cell_dir_entry *to, struct cell_dir_entry *from);

void cell_dir_entry_set(struct cell_dir_entry *cell_dir_entry, 
			struct checksum *checksum,
			uint32_t offset);

#endif
