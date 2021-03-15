#ifndef CELL_H
#define CELL_H

#include <stdint.h>
#include <stdbool.h>

// Forward declarations.
struct chunk;
struct cell_header;
struct checksum;

struct cell {
	struct cell_header *cell_header;
	char               *slab;
	uint32_t            slab_size;
};

void cell_init(struct cell *cell, char *slab, uint32_t slab_size);

void cell_add_chunk(struct cell *cell, 
		   struct checksum *checksum, 
		   char *buffer, 
		   uint32_t buffer_size);

bool cell_get_chunk(struct cell *cell,
		struct checksum *checksum,
		struct chunk *chunk);

bool cell_has_chunk(struct cell *cell,
		struct checksum *checksum);

#endif
