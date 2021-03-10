#ifndef CELL_BODY_ENTRY_H
#define CELL_BODY_ENTRY_H

#include <stdint.h>
#include <stdbool.h>

#include "theo/checksum.h"

struct cell_body_entry {
	struct checksum checksum;
	uint32_t free_space:1;
	uint32_t top_entry:1;
	uint32_t size:15;
	uint32_t previous_size:15;

};

void cell_body_entry_init(struct cell_body_entry *cell_body_entry);

bool cell_body_entry_free(struct cell_body_entry *cell_body_entry);

bool cell_body_entry_top(struct cell_body_entry *cell_body_entry);


#endif
