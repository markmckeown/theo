#include <string.h>
#include <assert.h>

#include "theo/cell_dir_block.h"
#include "theo/checksum.h"

void cell_dir_block_init(struct cell_dir_block *cell_dir_block) {
	memset(cell_dir_block, 0, sizeof(struct cell_dir_block));
	return;
}

void cell_dir_block_add(struct cell_dir_block *cell_dir_block, 
		        struct checksum *checksum, 
		        uint32_t chunk_size,
		        uint32_t offset) {
	assert(cell_dir_block->entry_count < CELL_DIR_BLOCK_ENTRIES);
	int i = 0;
	for (i = 0; i < CELL_DIR_BLOCK_ENTRIES; i++) {
		if (cell_dir_block->entries[i].size == 0) {
			memcpy(&cell_dir_block->entries[i].checksum, checksum, sizeof(struct checksum));
			cell_dir_block->entries[i].offset = offset;
			cell_dir_block->entries[i].size = chunk_size;
			cell_dir_block->entry_count++;
			break;
		}
	}
}

bool cell_dir_block_remove(struct cell_dir_block *cell_dir_block,
		          struct checksum *checksum) {
	bool ret = false;
	int i = 0;
	if (cell_dir_block->entry_count == 0) {
		goto out;
	}

	for (i = 0; i < CELL_DIR_BLOCK_ENTRIES; i++) {
		if (cell_dir_block->entries[i].size == 0) {
			continue;
		}
		if (checksum_compare(checksum, &cell_dir_block->entries[i].checksum) == 0) {
			cell_dir_entry_init(&cell_dir_block->entries[i]);
			cell_dir_block->entry_count--;
			ret = true;
			break;
		}
	}

out:
	return ret;	
}

bool cell_dir_block_full(struct cell_dir_block *cell_dir_block) {
	return (cell_dir_block->entry_count == CELL_DIR_BLOCK_ENTRIES);
}


bool cell_dir_block_has_chunk(struct cell_dir_block *cell_dir_block,
                             struct checksum *checksum, 
			     struct cell_dir_entry *cell_dir_entry){
	bool ret = false;
	int i = 0;
	if (cell_dir_block->entry_count == 0) {
		goto out;
	}

	for (i = 0; i < CELL_DIR_BLOCK_ENTRIES; i++) {
		if (cell_dir_block->entries[i].size == 0) {
			continue;
		}
		if (checksum_compare(checksum, &cell_dir_block->entries[i].checksum) == 0) {
			cell_dir_entry_copy(cell_dir_entry, &cell_dir_block->entries[i]);
			ret = true;
			break;
		}
	}

out:
	return ret;
}
