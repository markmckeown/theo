#ifndef CELL_DIR_OVERFLOW_H
#define CELL_DIR_OVERFLOW_H

#include <stdint.h>
#include <stdbool.h>

#include "theo/checksum.h"
#include "theo/cell_dir_entry.h"

struct cell_dir_overflow {
	uint16_t overflow_count;
};


void cell_dir_overflow_init(struct cell_dir_overflow *cell_dir_overflow);

uint32_t cell_dir_overflow_add(struct cell_dir_overflow *cell_dir_overflow, char *buffer_top,
		struct checksum *checksum, uint32_t chunk_size, uint32_t offset);

bool cell_dir_overflow_remove(struct cell_dir_overflow *cell_dir_overflow, char *buffer_top,
		struct checksum *checksum);

bool cell_dir_overflow_get_chunk(struct cell_dir_overflow *cell_dir_overflow, char *buffer_top,
		struct checksum *checksum, struct cell_dir_entry *cell_dir_entry);

#endif

