#include <string.h>
#include <assert.h>

#include "theo/cell_dir.h"

void cell_dir_init(struct cell_dir *cell_dir, char* buffer_start, uint32_t buffer_size) {
	assert(buffer_start != 0);  //LCOV_EXCL_LINE
	assert(cell_dir != 0);      //LCOV_EXCL_LINE
	assert(buffer_size > 0);    //LCOV_EXCL_LINE
	int i = 0;
	memset(cell_dir, 0, sizeof(struct cell_dir));
	for (i = 0; i < CELL_DIR_MAX_ENTRIES; i++) {
		cell_dir_entry_init(&cell_dir->entries[i]);
	}
	cell_dir->buffer_start = buffer_start;
	cell_dir->buffer_size = buffer_size;
	cell_dir->hole_size = buffer_size;
	cell_dir->hole_start = buffer_start;
	// cell_entries_count and next_slot should be null.
	return;
}
