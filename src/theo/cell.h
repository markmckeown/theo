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
#ifndef CELL_H
#define CELL_H

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

// Forward declarations.
struct chunk;
struct cell_header;
struct checksum;
struct cache_metrics;

struct cell {
	pthread_mutex_t     mutex;
	struct cell_header *cell_header;
	char               *slab;
	uint32_t            slab_size;
};

void cell_init(struct cell *cell, char *slab, uint32_t slab_size);

void cell_release(struct cell *cell);

bool cell_add_chunk(struct cell *cell, 
		   struct checksum *checksum, 
		   char *buffer, 
		   uint32_t buffer_size);

bool cell_get_chunk(struct cell *cell,
		struct checksum *checksum,
		struct chunk *chunk);

bool cell_has_chunk(struct cell *cell,
		struct checksum *checksum);

int32_t cell_entry_count(struct cell *cell);

int32_t cell_byte_count(struct cell *cell);

void cell_get_metrics(struct cell *cell, struct cache_metrics *cache_metrics);

#endif
