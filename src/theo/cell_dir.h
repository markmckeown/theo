#ifndef CELL_DIR_H
#define CELL_DIR_H

#define CELL_DIR_BLOCK_COUNT 256
#define CELL_DIR_BLOCK_CHECKSUM_BYTE 17

#include "theo/cell_dir_block.h"
#include "theo/cell_dir_entry.h"
#include "theo/cell_dir_overflow.h"

// Will hold some number of chunk references.
struct cell_dir {
	struct cell_dir_block blocks[CELL_DIR_BLOCK_COUNT];
	struct cell_dir_overflow overflow;
};

void cell_dir_init(struct cell_dir *cell_dir);

void cell_dir_add(struct cell_dir *cell_dir, 
		char* buffer_top,
		struct cell_dir_entry *cell_dir_entry);

bool cell_dir_remove(struct cell_dir *cell_dir,
		char* buffer_top,
		struct checksum *checksum);

bool cell_dir_get_chunk(struct cell_dir *cell_dir, 
		char* buffer_top, 
		struct checksum *checksum, 
		struct cell_dir_entry *cell_dir_entry);

#endif
