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

#include "theo/cell_dir.h"
#include "theo/msys.h"

void cell_dir_init(struct cell_dir *cell_dir)
{
	int i = 0;
	memset(cell_dir, 0, sizeof(struct cell_dir));
	for (i = 0; i < CELL_DIR_BLOCK_COUNT; i++) {
		cell_dir_block_init(&cell_dir->blocks[i]);
	}
	cell_dir_overflow_init(&cell_dir->overflow);
	return;
}

void cell_dir_add(struct cell_dir *cell_dir,
		  char *buffer_top, struct cell_dir_entry *cell_dir_entry)
{
	uint8_t block_no =
	    cell_dir_entry->checksum.bytes[CELL_DIR_BLOCK_CHECKSUM_BYTE];
	if (!cell_dir_block_add(&cell_dir->blocks[block_no], cell_dir_entry)) {
		cell_dir_overflow_add(&cell_dir->overflow, buffer_top,
				      cell_dir_entry);
		cell_dir_block_increment_overflow(&cell_dir->blocks[block_no]);
	}
	return;
}

bool cell_dir_remove(struct cell_dir *cell_dir,
		     char *buffer_top, struct checksum *checksum)
{
	bool ret = false;
	uint8_t block_no = checksum->bytes[CELL_DIR_BLOCK_CHECKSUM_BYTE];

	if (cell_dir_block_remove(&cell_dir->blocks[block_no], checksum)) {
		ret = true;
		goto out;
	}

	if (cell_dir_block_overflowed(&cell_dir->blocks[block_no])) {
		if (cell_dir_overflow_remove
		    (&cell_dir->overflow, buffer_top, checksum)) {
			cell_dir_block_decrement_overflow(&cell_dir->blocks
							  [block_no]);
			ret = true;
		}

	}
out:
	return ret;
}

bool cell_dir_get_chunk(struct cell_dir *cell_dir,
			char *buffer_top,
			struct checksum *checksum,
			struct cell_dir_entry *cell_dir_entry)
{
	bool ret = false;
	uint8_t block_no = checksum->bytes[CELL_DIR_BLOCK_CHECKSUM_BYTE];

	if (cell_dir_block_get_chunk
	    (&cell_dir->blocks[block_no], checksum, cell_dir_entry)) {
		ret = true;
		goto out;
	}

	if (cell_dir_block_overflowed(&cell_dir->blocks[block_no])) {
		ret =
		    cell_dir_overflow_get_chunk(&cell_dir->overflow, buffer_top,
						checksum, cell_dir_entry);
	}
out:
	return ret;
}
