#ifndef CELL_DIR_H
#define CELL_DIR_H

#define CELL_DIR_MAX_ENTRIES 32

#include "theo/cell_dir_entry.h"

// Will hold some number of chunk references.
struct cell_dir {
	struct cell_dir_entry entries[CELL_DIR_MAX_ENTRIES];
};

void cell_dir_init(struct cell_dir *cell_dir);

#endif
