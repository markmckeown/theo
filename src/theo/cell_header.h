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
#ifndef CELL_HEADER_H
#define CELL_HEADER_H

#include <stdbool.h>

#include "theo/cell_ic.h"
#include "theo/cell_dir.h"
#include "theo/cell_body.h"
#include "theo/msys.h"

#define CELL_HEADER_MAGIC 26061973

struct cell;
struct cell_body_entry;
struct checksum;

struct cell_header {
	uint32_t          magic;
	int32_t           entry_count;
	int32_t           byte_count;        
	struct cell_ic    cell_ic;
	struct cell_body  cell_body;
	struct cell_dir   cell_dir;
};

void cell_header_init(struct cell_header *cell_header, struct cell *cell);

bool cell_header_sane(struct cell_header *cell_header);

uint32_t cell_header_bytes_used(struct cell_header *cell_header);

void cell_header_remove(struct cell_header *cell_header,
		struct cell_body_entry *cell_body_entry);

void cell_header_add(struct cell_header *cell_header,
		struct cell_dir_entry *cell_dir_entry,
		uint32_t size);

int32_t cell_header_entry_count(struct cell_header *cell_header);

int32_t cell_header_byte_count(struct cell_header *cell_header);

#endif
