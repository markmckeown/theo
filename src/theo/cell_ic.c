#include <string.h>

#include "theo/cell_ic.h"


void cell_ic_init(struct cell_ic *cell_ic) {
	memset(cell_ic, 0, sizeof(struct cell_ic));
	return;
}

int cell_ic_sanity(struct cell_ic *cell_ic) {
	if (cell_ic->entry_code != cell_ic->exit_code) {
		return -1;
	}
	return 0;
}

uint32_t cell_ic_on_entry(struct cell_ic *cell_ic) {
	cell_ic->entry_code++;
	// Now before proceeding issue a full memory barrier
	__sync_synchronize();
	return cell_ic->entry_code;
}

uint32_t cell_ic_on_exit(struct cell_ic *cell_ic) {
	// Flush everything done to memory using full memory barrier.
	__sync_synchronize();
	cell_ic->exit_code = cell_ic->entry_code;
	// Make sure exit_code is flushed to memory.
	__sync_synchronize();
	return cell_ic->exit_code;
}

int cell_ic_padding() {
	if (sizeof(struct cell_ic) != 2 * sizeof(uint32_t)) {
		return -1;
	}
	return 0;
}
