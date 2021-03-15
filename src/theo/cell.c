#include <string.h>

#include "theo/cell.h"
#include "theo/chunk.h"
#include "theo/cell_header.h"
#include "theo/cell_body_entry.h"
#include "theo/msys.h"

void cell_init(struct cell *cell, char *slab, uint32_t slab_size) {
	ALWAYS(slab_size > sizeof(struct cell_header));
	memset(cell, 0, sizeof(struct cell));
	cell->slab = slab;
	cell->slab_size = slab_size;
	cell->cell_header = (struct cell_header *) (slab + (slab_size - sizeof(struct cell_header)));
	if (!cell_header_sane(cell->cell_header)) {
		cell_header_init(cell->cell_header, cell);
	}

	return;
}

void cell_add_chunk(struct cell *cell, 
		   struct checksum *checksum, 
		   char *buffer, 
		   uint32_t buffer_size) {
	struct cell_dir_entry cell_dir_entry;

	cell_dir_entry_init(&cell_dir_entry);
	cell_ic_on_entry(&cell->cell_header->cell_ic);

	cell_body_add(&cell->cell_header->cell_body, cell,
			checksum, buffer, buffer_size, &cell_dir_entry);

	cell_ic_on_exit(&cell->cell_header->cell_ic);
	return;
}




bool cell_get_chunk(struct cell *cell,
		struct checksum *checksum,
		struct chunk *chunk) {
	bool ret = true;
	struct cell_dir_entry cell_dir_entry;
	struct cell_body_entry *cell_body_entry;
	char *ptr;

	cell_dir_entry_init(&cell_dir_entry);
	if (!cell_dir_get_chunk(&cell->cell_header->cell_dir, 
			cell->slab + (cell->slab_size - sizeof(struct cell_header)),
			checksum,
			&cell_dir_entry)) {
		ret = false;
		goto out;
	}

	ptr = cell->slab + cell_dir_entry.offset;
	cell_body_entry = (struct cell_body_entry *) ptr;
	ptr += sizeof(struct cell_body_entry);
	chunk_set(chunk, &cell_dir_entry.checksum, ptr, cell_body_entry->size);

out:	
	return ret;
}

bool cell_has_chunk(struct cell *cell,
		struct checksum *checksum) {
	bool ret;
	struct cell_dir_entry cell_dir_entry;

	cell_dir_entry_init(&cell_dir_entry);
	ret = cell_dir_get_chunk(&cell->cell_header->cell_dir, 
			cell->slab + (cell->slab_size - sizeof(struct cell_header)),
			checksum,
			&cell_dir_entry);
	return ret;
}
