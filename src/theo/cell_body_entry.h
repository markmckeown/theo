/**
    Copyright 2021 WANdisco

    This file is part of Libtheo.

    Libtheo is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
*/
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
