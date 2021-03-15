#ifndef CELL_BODY_H
#define CELL_BODY_H

#include <stdint.h>

struct cell;
struct chunk;
struct cell_dir_entry;
struct checksum;

struct cell_body {
	uint32_t top_entry_offset;
	uint32_t next_entry_offset;
};

void cell_body_init(struct cell_body *cell_body, struct cell *cell);

void cell_body_add(struct cell_body *cell_body,
		struct cell *cell,
		struct checksum *checksum,
		char *buffer,
		uint32_t buffer_size,
		struct cell_dir_entry *cell_dir_entry);


#endif
