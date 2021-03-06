#include <string.h>

#include "theo/cell_dir.h"
#include "theo/msys.h"

void cell_dir_init(struct cell_dir *cell_dir) {
	int i = 0;
	memset(cell_dir, 0, sizeof(struct cell_dir));
	for (i = 0; i < CELL_DIR_BLOCK_COUNT; i++) {
		cell_dir_block_init(&cell_dir->blocks[i]);
	}
	cell_dir_overflow_init(&cell_dir->overflow);
	return;
}


void cell_dir_add(struct cell_dir *cell_dir, struct checksum *checksum, uint32_t chunk_size, uint32_t offset) {
	uint32_t block_no = checksum->bytes[CELL_DIR_BLOCK_CHECKSUM_BYTE];
	ALWAYS(cell_dir_block_full(&cell_dir->blocks[block_no]) != 0);
	cell_dir_block_add(&cell_dir->blocks[block_no], checksum, chunk_size, offset);
	return;
}

bool cell_dir_has_chunk(struct cell_dir *cell_dir, struct checksum *checksum, struct cell_dir_entry *cell_dir_entry) {
	uint32_t block_no = checksum->bytes[CELL_DIR_BLOCK_CHECKSUM_BYTE];
	return cell_dir_block_has_chunk(&cell_dir->blocks[block_no], checksum, cell_dir_entry);
}
