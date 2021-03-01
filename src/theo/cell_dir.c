#include <string.h>

#include "theo/cell_dir.h"

void cell_dir_init(struct cell_dir *cell_dir) {
	int i = 0;
	memset(cell_dir, 0, sizeof(struct cell_dir));
	for (i = 0; i < CELL_DIR_MAX_ENTRIES; i++) {
		cell_dir_entry_init(&cell_dir->entries[i]);
	}
	return;
}
