#include <string.h>

#include "theo/cell_header.h"


void cell_header_init(struct cell_header *cell_header, uint32_t buffer_size) {
	memset(cell_header, 0, sizeof(struct cell_header));
	cell_ic_init(&cell_header->cell_ic);
	cell_dir_init(&cell_header->cell_dir);
	cell_header->buffer_size = buffer_size;
	return;
}

int cell_header_sanity(struct cell_header *cell_header) {
	return cell_ic_sanity(&cell_header->cell_ic);
}

