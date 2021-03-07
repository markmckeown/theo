#ifndef CELL_HEADER_H
#define CELL_HEADER_H

#include <stdbool.h>

#include "theo/cell_ic.h"
#include "theo/cell_dir.h"

#define CELL_HEADER_MAGIC 26061973

struct cell_header {
	uint32_t magic;
	struct cell_ic cell_ic;
	struct cell_dir cell_dir;
};

void cell_header_init(struct cell_header *cell_header);

bool cell_header_sane(struct cell_header *cell_header);

#endif
