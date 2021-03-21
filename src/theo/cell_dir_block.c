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

#include "theo/cell_dir_block.h"
#include "theo/checksum.h"
#include "theo/msys.h"

void cell_dir_block_init(struct cell_dir_block *cell_dir_block) {
	memset(cell_dir_block, 0, sizeof(struct cell_dir_block));
	return;
}

bool cell_dir_block_add(struct cell_dir_block *cell_dir_block, 
		        struct cell_dir_entry *cell_dir_entry) {
	bool ret = false;
	int i = 0;

	for (i = 0; i < CELL_DIR_BLOCK_ENTRIES; i++) {
		if (cell_dir_block->entries[i].set == 0) {
			cell_dir_entry_copy(&cell_dir_block->entries[i], cell_dir_entry);
			cell_dir_block->entry_count++;
			ret = true;
			break;
		}
	}

	return ret;	
}


struct cell_dir_entry* cell_dir_block_find(struct cell_dir_block *cell_dir_block,
                          struct checksum *checksum) {
	struct cell_dir_entry *entry = 0;
	int i = 0;

	if (cell_dir_block->entry_count == 0) {
		goto out;
	}
	for (i = 0; i < CELL_DIR_BLOCK_ENTRIES; i++) {
		if (cell_dir_block->entries[i].set == 0) {
			continue;
		}
		if (checksum_compare(checksum, &cell_dir_block->entries[i].checksum) == 0) {
			entry = &cell_dir_block->entries[i];
			break;
		}
	}

out:
	return entry;
}

bool cell_dir_block_remove(struct cell_dir_block *cell_dir_block,
		          struct checksum *checksum) {
	bool ret = false;
	struct cell_dir_entry *entry = 0;

	entry = cell_dir_block_find(cell_dir_block, checksum);
	if (entry != 0) {
		cell_dir_entry_init(entry);
		cell_dir_block->entry_count--;
		ret = true;
	}

	return ret;	
}

bool cell_dir_block_full(struct cell_dir_block *cell_dir_block) {
	return (cell_dir_block->entry_count == CELL_DIR_BLOCK_ENTRIES);
}


bool cell_dir_block_get_chunk(struct cell_dir_block *cell_dir_block,
                             struct checksum *checksum, 
			     struct cell_dir_entry *cell_dir_entry){
	bool ret = false;
	struct cell_dir_entry *entry = 0;

	entry = cell_dir_block_find(cell_dir_block, checksum);
	if (entry != 0) {
		cell_dir_entry_copy(cell_dir_entry, entry);
		ret = true;
	}

	return ret;
}


bool cell_dir_block_overflowed(struct cell_dir_block *cell_dir_block) {
	return (cell_dir_block->overflow_count > 0);
}

uint32_t cell_dir_block_increment_overflow(struct cell_dir_block *cell_dir_block) {
	cell_dir_block->overflow_count++;
	return cell_dir_block->overflow_count;
}

uint32_t cell_dir_block_decrement_overflow(struct cell_dir_block *cell_dir_block) {
	ALWAYS(cell_dir_block->overflow_count > 0);
	cell_dir_block->overflow_count--;
	return cell_dir_block->overflow_count;
}       
