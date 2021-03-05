#include <string.h>

#include "theo/cell_dir_entry.h"


void cell_dir_entry_init(struct cell_dir_entry *cell_dir_entry) {
	memset(cell_dir_entry, 0, sizeof(struct cell_dir_entry));
	checksum_init(&cell_dir_entry->checksum);
	return;
}

void cell_dir_entry_copy(struct cell_dir_entry *to, struct cell_dir_entry *from) {
	memcpy(to, from, sizeof(struct cell_dir_entry));
	return;
}
