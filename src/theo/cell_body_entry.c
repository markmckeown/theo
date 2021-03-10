#include <string.h>

#include "theo/cell_body_entry.h"

void cell_body_entry_init(struct cell_body_entry *cell_body_entry) {
	memset(cell_body_entry, 0, sizeof(struct cell_body_entry));
	return;
}


bool cell_body_entry_free(struct cell_body_entry *cell_body_entry) {
        return (cell_body_entry->free_space == 1);
}

bool cell_body_entry_top(struct cell_body_entry *cell_body_entry) {
	return (cell_body_entry->top_entry == 1);
}

