#ifndef CELL_HEADER_H
#define CELL_HEADER_H

#include <stdbool.h>

#include "theo/cell_ic.h"
#include "theo/cell_dir.h"
#include "theo/cell_body.h"

#define CELL_HEADER_MAGIC 26061973

struct cell;

struct cell_header {
	uint32_t          magic;
	struct cell_ic    cell_ic;
	struct cell_body  cell_body;
	struct cell_dir   cell_dir;
};

void cell_header_init(struct cell_header *cell_header, struct cell *cell);

bool cell_header_sane(struct cell_header *cell_header);

uint32_t cell_header_bytes_used(struct cell_header *cell_header);

#endif
