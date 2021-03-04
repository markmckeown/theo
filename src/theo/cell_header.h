#ifndef CELL_HEADER_H
#define CELL_HEADER_H

#include "theo/cell_ic.h"
#include "theo/cell_dir.h"

struct cell_header {
	struct cell_ic cell_ic;
	struct cell_dir cell_dir;
};

void cell_header_init(struct cell_header *cell_header, char* buffer_start, uint32_t buffer_size);

int cell_header_sanity(struct cell_header *cell_header);

#endif
