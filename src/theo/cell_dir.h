#ifndef CELL_DIR_H
#define CELL_DIR_H

#define CELL_DIR_BLOCK_COUNT 256
#define CELL_DIR_BLOCK_CHECKSUM_BYTE 17

#include "theo/cell_dir_block.h"
#include "theo/cell_dir_entry.h"

// Will hold some number of chunk references.
struct cell_dir {
	struct cell_dir_block blocks[CELL_DIR_BLOCK_COUNT];
};

void cell_dir_init(struct cell_dir *cell_dir);

void cell_dir_add(struct cell_dir *cell_dir, struct checksum *checksum, uint32_t chunk_size, uint32_t offset);

bool cell_dir_has_chunk(struct cell_dir *cell_dir, struct checksum *checksum, struct cell_dir_entry *cell_dir_entry);

#endif
