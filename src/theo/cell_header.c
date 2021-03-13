#include <string.h>

#include "theo/cell_header.h"


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

