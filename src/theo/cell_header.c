#include <string.h>

#include "theo/cell_header.h"


void cell_header_init(struct cell_header *cell_header) {
	memset(cell_header, 0, sizeof(struct cell_header));
	cell_ic_init(&cell_header->cell_ic);
	cell_dir_init(&cell_header->cell_dir);
	cell_header->magic = CELL_HEADER_MAGIC;
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


