#ifndef CELL_DIR_H
#define CELL_DIR_H

#define CELL_DIR_MAX_ENTRIES 32

#include <string.h>

#include "theo/cell_dir_entry.h"

// Will hold some number of chunk references.
struct cell_dir {
	struct cell_dir_entry entries[CELL_DIR_MAX_ENTRIES];
	char* buffer_start;
	char* hole_start;
	uint32_t buffer_size;
	uint32_t next_slot;
	uint32_t cell_entries_count;
	uint32_t hole_size;	
};

void cell_dir_init(struct cell_dir *cell_dir, char* buffer_start, uint32_t buffer_size);

#endif
