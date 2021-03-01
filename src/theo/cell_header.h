#ifndef CELL_HEADER_H
#define CELL_HEADER_H

#include "theo/cell_ic.h"

struct cell_header {
	struct cell_ic cell_ic;
};

void cell_header_init(struct cell_header *cell_header);

int cell_header_sanity(struct cell_header *cell_header);

#endif
