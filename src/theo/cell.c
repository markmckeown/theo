#include <string.h>

#include "theo/cell.h"
#include "theo/msys.h"

void cell_init(struct cell *cell, char *slab, uint32_t slab_size) {
	ALWAYS(slab_size > sizeof(struct cell_header));
	memset(cell, 0, sizeof(struct cell));
	cell->cell_header = (struct cell_header *) (slab + (slab_size - sizeof(struct cell_header)));
	if (!cell_header_sane(cell->cell_header)) {
		cell_header_init(cell->cell_header);
	}
	cell->slab = slab;
	cell->slab_size = slab_size;

	return;
}
