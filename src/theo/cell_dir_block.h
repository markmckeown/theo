#ifndef CELL_DIR_BLOCK_H
#define CELL_DIR_BLOCK_H

#define CELL_DIR_BLOCK_ENTRIES 3

#include <stdbool.h>

#include "theo/cell_dir_entry.h"

// Will hold some number of chunk references.
struct cell_dir_block {
	struct cell_dir_entry entries[CELL_DIR_BLOCK_ENTRIES];
	uint16_t entry_count :3;
	uint16_t overflow_count :13;
};

void cell_dir_block_init(struct cell_dir_block *cell_dir_block);

void cell_dir_block_add(struct cell_dir_block *cell_dir_block, 
		       struct checksum *checksum, 
		       uint32_t chunk_size,
		       uint32_t offset);

bool cell_dir_block_remove(struct cell_dir_block *cell_dir_block,
		          struct checksum *checksum);

bool cell_dir_block_has_chunk(struct cell_dir_block *cell_dir_block, 
 	                     struct checksum *checksum, 
	  		     struct cell_dir_entry *cell_dir_entry);

bool cell_dir_block_full(struct cell_dir_block *cell_dir_block);

#endif
