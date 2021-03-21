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
#ifndef CELL_DIR_BLOCK_H
#define CELL_DIR_BLOCK_H

#define CELL_DIR_BLOCK_ENTRIES 3

#include <stdbool.h>

#include "theo/cell_dir_entry.h"

// Will hold some number of chunk references.
struct cell_dir_block {
	struct cell_dir_entry entries[CELL_DIR_BLOCK_ENTRIES];
	uint32_t entry_count :4;        // Max 16 entries
	uint32_t overflow_count :12;    // This block can overflow by 4096
	uint32_t padding :16;
};

void cell_dir_block_init(struct cell_dir_block *cell_dir_block);

bool cell_dir_block_add(struct cell_dir_block *cell_dir_block, 
		       struct cell_dir_entry  *cell_dir_entry);

bool cell_dir_block_remove(struct cell_dir_block *cell_dir_block,
		          struct checksum *checksum);

bool cell_dir_block_get_chunk(struct cell_dir_block *cell_dir_block, 
 	                     struct checksum *checksum, 
	  		     struct cell_dir_entry *cell_dir_entry);

bool cell_dir_block_full(struct cell_dir_block *cell_dir_block);

bool cell_dir_block_overflowed(struct cell_dir_block *cell_dir_block);

uint32_t cell_dir_block_increment_overflow(struct cell_dir_block *cell_dir_block);

uint32_t cell_dir_block_decrement_overflow(struct cell_dir_block *cell_dir_block);

#endif
