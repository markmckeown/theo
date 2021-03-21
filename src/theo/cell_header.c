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

#include "theo/cell_header.h"
#include "theo/cell_body_entry.h"
#include "theo/cell_dir.h"


void cell_header_init(struct cell_header *cell_header, struct cell *cell) {
	memset(cell_header, 0, sizeof(struct cell_header));
	cell_ic_init(&cell_header->cell_ic);
	cell_dir_init(&cell_header->cell_dir);
	cell_body_init(&cell_header->cell_body, cell);
	__sync_synchronize();
	cell_header->magic = CELL_HEADER_MAGIC;
	__sync_synchronize();
	return;
}

bool cell_header_sane(struct cell_header *cell_header) {
	bool ret = true;

	if (cell_header->magic != CELL_HEADER_MAGIC) {
		ret  = false;
		goto out;
	}

	ret = cell_ic_sane(&cell_header->cell_ic);

out:	
	return ret;
}

uint32_t cell_header_bytes_used(struct cell_header *cell_header) {
	uint32_t header_size;
	header_size = sizeof(struct cell_header) +
	      	cell_header->cell_dir.overflow.overflow_count * sizeof(struct cell_dir_entry);
	return header_size;
}


void cell_header_remove(struct cell_header *cell_header,
		struct cell_body_entry *cell_body_entry) {
	__attribute__((unused)) bool ret = false;

	ret = cell_dir_remove(&cell_header->cell_dir, (char *) cell_header,
			&cell_body_entry->checksum);
	ALWAYS(ret); // When called there should always be something to remove.
	cell_header->entry_count--;
	ALWAYS(cell_header->entry_count > 0);
	cell_header->byte_count = cell_header->byte_count - cell_body_entry->size;
	ALWAYS(cell_header->byte_count> 0);
	return;
}

void cell_header_add(struct cell_header *cell_header,
		struct cell_dir_entry *cell_dir_entry,
		uint32_t size) {

	cell_dir_add(&cell_header->cell_dir, (char *) cell_header, 
			cell_dir_entry);
	cell_header->entry_count++;
	cell_header->byte_count = cell_header->byte_count + size;
	return;
}


int32_t cell_header_byte_count(struct cell_header *cell_header) {
	return cell_header->byte_count;
}

int32_t cell_header_entry_count(struct cell_header *cell_header) {
	return cell_header->entry_count;
}
