#ifndef CELL_DIR_ENTRY_H
#define CELL_DIR_ENTRY_H

#include "theo/checksum.h" 

// Reference for a chunk in the cell.
struct cell_dir_entry {
	// checksum of chunk
	struct checksum checksum;
	// offset into the cell for the data.
	uint32_t offset :31;
	uint32_t set    : 1;
};

void cell_dir_entry_init(struct cell_dir_entry *cell_dir_entry);

void cell_dir_entry_copy(struct cell_dir_entry *to, struct cell_dir_entry *from);

void cell_dir_entry_set(struct cell_dir_entry *cell_dir_entry, 
			struct checksum *checksum,
			uint32_t offset);

#endif
