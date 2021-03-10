#ifndef CELL_H
#define CELL_H

#include "theo/cell_header.h"

struct cell {
	struct cell_header *cell_header;
	char               *slab;
	uint32_t            slab_size;
};

void cell_init(struct cell *cell, char *slab, uint32_t slab_size);

int cell_add_chunk(struct cell *cell, 
		   struct checksum *checksum, 
		   char *buffer, 
		   uint32_t buffer_size);

#endif
