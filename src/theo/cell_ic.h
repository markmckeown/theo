#ifndef CELL_IC_H
#define CELL_IC_H

#include <stdint.h>
#include <stdbool.h>

// Cell Integrity Code
struct cell_ic {
	uint32_t entry_code;
	uint32_t exit_code;
};


void cell_ic_init(struct cell_ic *cell_ic);

bool cell_ic_sane(struct cell_ic *cell_ic);

uint32_t cell_ic_on_entry(struct cell_ic *cell_ic);

uint32_t cell_ic_on_exit(struct cell_ic *cell_ic);

int cell_ic_padding();

#endif
