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
#ifndef CACHE_H
#define CACHE_H

#include <stdint.h>
#include <stdbool.h>

#define CACHE_CELL_SIZE 1024*1024ul

// Forward declaration.
struct cell;
struct cache_header;
struct cache_metrics;
struct checksum;
struct chunk;

struct cache {
	struct cell           *cells;
	struct cache_header   *cache_header;
	char                  *cache_buffer;
	uint64_t               cache_buffer_size;
};

int cache_init(struct cache *cache,
		char         *cache_buffer,
		uint64_t      cache_buffer_size);

bool cache_add_chunk(struct cache *cache, struct checksum *checksum,
		char *buffer, uint32_t buffer_size);

bool cache_has_chunk(struct cache *cache, struct checksum *checksum);

bool cache_get_chunk(struct cache *cache, struct checksum *checksum, 
		struct chunk *chunk);

void cache_release(struct cache *cache);

void cache_get_metrics(struct cache *cache, 
		struct cache_metrics *cache_metrics);

#endif
